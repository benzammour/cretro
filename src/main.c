#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>

#include "lcd.h"
#include "ch8.h"
#include "sound.h"
#include "timer.h"


int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: ./ch8 <DELAY> <ROM>\n");
	}
	
	machine_init();

	const uint16_t DELAY = strtol(argv[1], NULL, 10);
	const char* ROM_FILE_NAME = argv[2];

	uint8_t ret = rom_load(ROM_FILE_NAME);
	if (ret) {
		fprintf(stderr, "Failed to load ROM\n");
		exit(-1);
	}
	printf("Successfully initialized ROM\n");

	ret = lcd_init();
	if (ret) {
		fprintf(stderr, "Failed to initialize LCD\n");
		exit(-3);
	}
	printf("Successfully initialized LCD\n");

	int videoPitch = sizeof(m.video[0]) * VIDEO_WIDTH;

	struct timeval clock_prev;
	gettimeofday(&clock_prev, NULL);

	// create timer thread to decrease delay- and sound timer
	pthread_t tid;
	pthread_create(&tid, NULL, &timer_update_callback, NULL);

	uint8_t running = 1;
	while (running) {
		handle_sound();

		struct timeval clock_now;
	    gettimeofday(&clock_now, NULL);

		running = lcd_process_input();

		uint64_t dt = timediff_ms(&clock_now, &clock_prev);
		if (dt > DELAY) {
			if (cpu_step()) {
				break;
			}

			if (lcd_step(m.video, videoPitch)) {
				break;
			}
			clock_prev = clock_now;
		}
	}

	SDL_Quit();

	return 0;
}
