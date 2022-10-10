#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <math.h>
#include "cpu.h"

int sound_init(void);
void handle_sound(void);

#endif
