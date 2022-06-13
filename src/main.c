#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
#include "ch8.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: ./ch8 <ROM>\n");
	}

	const char* romFileName = argv[1];
	uint8_t ret = rom_load(romFileName);
	if (ret) {
		fprintf(stderr, "Failed to load ROM\n");
		exit(-1);
	}
	printf("Successfully initialized ROM\n");


	ret = lcd_load();
	if (ret) {
		fprintf(stderr, "Failed to initialize LCD\n");
		exit(-3);
	}
	printf("Successfully initialized LCD\n");


	uint8_t x = 0;
	while(1) {
		if (lcd_step()) {
			break;
		}

		x++;
	}

	printf("%d\n", x);
	SDL_Quit();

	return 0;
}
