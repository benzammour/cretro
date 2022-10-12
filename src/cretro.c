#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

#include "lcd.h"
#include "cpu.h"
#include "sound.h"
#include "timer.h"
#include "cli.h"

int main(int argc, char** argv) {
    config_t conf = cli_config_default();

	if (cli_config_handle(&conf, argc, argv))
        return EXIT_FAILURE;

	machine_init();

	rom_load(conf.rom);
	printf("Successfully initialized ROM\n");

	if (lcd_init()) {
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

	bool running = true;
	while (running) {
		running = lcd_process_input();

		handle_sound();

		struct timeval clock_now;
	    gettimeofday(&clock_now, NULL);
		long dt = timediff_ms(&clock_now, &cpu_clock_prev);
		if (dt > conf.delay) {
			if (cpu_step() || lcd_step(m.video, videoPitch)) {
				break;
			}

			cpu_clock_prev = clock_now;
		}
	}

	SDL_Quit();

    return EXIT_SUCCESS;
}
