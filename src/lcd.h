#ifndef LCD_H
#define LCD_H
#include <stdint.h>
#include <SDL2/SDL.h>
#include "ch8.h"


uint8_t lcd_init();
uint8_t lcd_process_input();
uint8_t lcd_step(void const* buffer, int pitch);

#endif