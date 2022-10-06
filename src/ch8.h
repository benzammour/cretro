#ifndef CH8_H
#define CH8_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define FONTSET_SIZE 80
#define FONTSET_START_ADDRESS 0x00
#define START_ADDRESS 0x200
#define REGISTER_COUNT 16
#define STACK_LEVELS 16
#define SCALE_FACTOR 100
#define VIDEO_HEIGHT 32
#define VIDEO_WIDTH 64
#define KEY_SIZE 16
#define SCREEN_DIMENSIONS VIDEO_WIDTH * VIDEO_HEIGHT

#define GET_X(opcode) ((opcode >> 8) & 0x000F)
#define GET_Y(opcode) ((opcode >> 4) & 0x000F)
#define GET_N(opcode) (opcode & 0x000F)
#define GET_KK(opcode) (opcode & 0x00FF)
#define GET_NNN(opcode) (opcode & 0x0FFF)


uint8_t rom_load(const char *filename);
uint8_t *get_rom_bytes();

void machine_init(void);
uint16_t cpu_get_cycle_count();
uint8_t cpu_step(void);

// Do nothing
void ILLEGAL_OPCODE();

struct machine_t {
	uint8_t memory[4096];
	uint8_t registers[REGISTER_COUNT];
	uint16_t stack[STACK_LEVELS];
	uint32_t video[SCREEN_DIMENSIONS];
	uint8_t keys[KEY_SIZE];

	uint16_t index;
	uint16_t SP;
	uint16_t PC;
	uint16_t cycle_count;
	uint16_t opcode;
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint8_t should_beep;
};

extern struct machine_t m;

typedef void (*op_function)();

#endif
