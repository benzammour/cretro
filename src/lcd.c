#include <stdio.h>
#include <stdbool.h>

#include "lcd.h"

static SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

uint8_t lcd_init() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(
		"ch8 - Chip 8 Emulator",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640, 320,
		SDL_WINDOW_INPUT_FOCUS
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

	return 0;
}

uint8_t lcd_process_input() {
	SDL_Event e;
	int running = 1;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			running = SDL_FALSE;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = SDL_FALSE;
					break;
				case SDLK_x:
					m.keys[0] = 1;
					break;
				case SDLK_1:
					m.keys[1] = 1;
					break;
				case SDLK_2:
					m.keys[2] = 1;
					break;
				case SDLK_3:
					m.keys[3] = 1;
					break;
				case SDLK_q:
					m.keys[4] = 1;
					break;
				case SDLK_w:
					m.keys[5] = 1;
					break;
				case SDLK_e:
					m.keys[6] = 1;
					break;
				case SDLK_a:
					m.keys[7] = 1;
					break;
				case SDLK_s:
					m.keys[8] = 1;
					break;
				case SDLK_d:
					m.keys[9] = 1;
					break;
				case SDLK_y:
					m.keys[0xA] = 1;
					break;
				case SDLK_c:
					m.keys[0xB] = 1;
					break;
				case SDLK_4:
					m.keys[0xC] = 1;
					break;
				case SDLK_r:
					m.keys[0xD] = 1;
					break;
				case SDLK_f:
					m.keys[0xE] = 1;
					break;
				case SDLK_v:
					m.keys[0xF] = 1;
					break;
			}
			break;
		case SDL_KEYUP:
			switch (e.key.keysym.sym) {
				case SDLK_x:
					m.keys[0] = 0;
					break;
				case SDLK_1:
					m.keys[1] = 0;
					break;
				case SDLK_2:
					m.keys[2] = 0;
					break;
				case SDLK_3:
					m.keys[3] = 0;
					break;
				case SDLK_q:
					m.keys[4] = 0;
					break;
				case SDLK_w:
					m.keys[5] = 0;
					break;
				case SDLK_e:
					m.keys[6] = 0;
					break;
				case SDLK_a:
					m.keys[7] = 0;
					break;
				case SDLK_s:
					m.keys[8] = 0;
					break;
				case SDLK_d:
					m.keys[9] = 0;
					break;
				case SDLK_y:
					m.keys[0xA] = 0;
					break;
				case SDLK_c:
					m.keys[0xB] = 0;
					break;
				case SDLK_4:
					m.keys[0xC] = 0;
					break;
				case SDLK_r:
					m.keys[0xD] = 0;
					break;
				case SDLK_f:
					m.keys[0xE] = 0;
					break;
				case SDLK_v:
					m.keys[0xF] = 0;
					break;
		}
			break;
		}
	}

	return running;
}

uint8_t lcd_step(void const *buffer, int pitch) {
	SDL_UpdateTexture(texture, NULL, buffer, pitch);
	SDL_SetTextureColorMod(texture, 69, 170, 242);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	return 0;
}
