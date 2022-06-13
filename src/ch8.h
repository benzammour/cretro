#ifndef CH8_H
#define CH8_H
#include <stdint.h>
#include <stdlib.h>

uint8_t rom_load(const char *filename);
uint8_t *get_rom_bytes();

void cpu_init(void);
uint16_t cpu_get_cycle_count();
uint8_t cpu_step(void);

#endif
