#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
#include "ch8.h"
#include <sys/time.h>

uint64_t current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    uint64_t milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: ./ch8 <ROM>\n");
	}
	
	machine_init();

	const char* romFileName = argv[1];
	uint8_t ret = rom_load(romFileName);
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

	uint8_t running = 1;
	uint64_t before = current_timestamp();
	while (running) {
		running = lcd_process_input();

		int trigger = (1000 / 60);
		uint64_t currentTime = current_timestamp();

		long dt = currentTime - before;
		if (dt > trigger) {
			before = currentTime;

			if (cpu_step()) {
				break;
			}

			if (lcd_step(m.video, videoPitch)) {
				break;
			}
		}
	}

	SDL_Quit();

	return 0;
}
