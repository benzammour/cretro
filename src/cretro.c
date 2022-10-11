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
		fprintf(stderr, "Usage: ./cretro <CPU-FREQUENCY-IN-HZ> <ROM>\n");
        return EXIT_FAILURE;
	}
	
	machine_init();

    long inputHertz = strtol(argv[1], NULL, 10);
    if (inputHertz > 1000000L) {
        fprintf(stderr, "Provided frequency %ld Hz is higher than allowed frequency %ld Hz\n", inputHertz, 1000000L);
        return EXIT_FAILURE;
    }
	const uint32_t USECOND_DELAY = (uint32_t) (1000000L / inputHertz);
	const char* ROM_FILE_NAME = argv[2];

	rom_load(ROM_FILE_NAME);
	printf("Successfully initialized ROM\n");

	lcd_init();
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
		long dt = timediff_us(&clock_now, &cpu_clock_prev);
		if (dt > USECOND_DELAY) {
			cpu_step();
			lcd_step(m.video, videoPitch);
			cpu_clock_prev = clock_now;
		}
	}

	SDL_Quit();

    return EXIT_SUCCESS;
}
