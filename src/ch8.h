#ifndef CH8_H
#define CH8_H
#include <stdint.h>
#include <stdlib.h>

#define START_ADDRESS 0x200

uint8_t rom_load(const char *filename);
uint8_t *get_rom_bytes();

void cpu_init(void);
uint16_t cpu_get_cycle_count();
uint8_t cpu_step(void);

// Do nothing
void NOOP();

struct CPU {
	uint8_t REG;

	uint16_t index;
	uint16_t SP;
	uint16_t PC;
	uint16_t cycle_count;
	uint16_t opcode;
};

typedef void (*op_function)();


#endif
