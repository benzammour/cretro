#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>

#include "lcd.h"
#include "cpu.h"
#include "sound.h"
#include "timer.h"


int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: ./cretro <DELAY> <ROM>\n");
        return EXIT_FAILURE;
	}
	
	machine_init();

    long inputDelay = strtol(argv[1], NULL, 10);
    if (inputDelay > UINT16_MAX) {
        fprintf(stderr, "Provided delay %ld is higher than allowed delay %d\n", inputDelay, UINT16_MAX);
        return EXIT_FAILURE;
    }
	const uint16_t DELAY = (uint16_t) inputDelay;
	const char* ROM_FILE_NAME = argv[2];

	rom_load(ROM_FILE_NAME);
	printf("Successfully initialized ROM\n");

	uint8_t ret = lcd_init();
	if (ret) {
		fprintf(stderr, "Failed to initialize LCD\n");
		exit(-3);
	}
	printf("Successfully initialized LCD\n");

	int videoPitch = sizeof(m.video[0]) * VIDEO_WIDTH;

	struct timeval cpu_clock_prev;
	gettimeofday(&cpu_clock_prev, NULL);

	// create timer thread to decrease delay- and sound timer
	pthread_t tid;
	pthread_create(&tid, NULL, &timer_update_callback, NULL);

	uint8_t running = 1;
	while (running) {
		running = lcd_process_input();

		handle_sound();

		struct timeval clock_now;
	    gettimeofday(&clock_now, NULL);
		long dt = timediff_ms(&clock_now, &cpu_clock_prev);
		if (dt > DELAY) {
			if (cpu_step() || lcd_step(m.video, videoPitch)) {
				break;
			}

			cpu_clock_prev = clock_now;
		}
	}

	SDL_Quit();

    return EXIT_SUCCESS;
}
