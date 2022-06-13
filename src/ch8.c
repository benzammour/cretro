#include <stdio.h>
#include "ch8.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define START_ADDRESS 0x200

struct CPU {
	uint8_t REG;

	uint16_t index;
	uint16_t SP;
	uint16_t PC;
	uint16_t cycle_count;
	uint16_t opcode;
};

struct CPU c;
uint8_t rom_buffer[4096];


/* ------------------ ROM Funcs */
uint8_t rom_load(const char *filename) {
    FILE *f;
	uint8_t *tmp_buf;
	uint16_t numbytes;

	// Open ROM
    f = fopen(filename, "r");
	if(f == NULL) {
		return 1;
	}

	// Get the number of bytes
	fseek(f, 0L, SEEK_END);
	numbytes = ftell(f);
	
	// reset pointer to beginning of file
	fseek(f, 0L, SEEK_SET);	
	
	// grab sufficient memory for the buffer 
	tmp_buf = (uint8_t*) calloc(numbytes, sizeof(uint8_t));	
	if(tmp_buf == NULL) {
		return 1;
	}

	fread(tmp_buf, sizeof(uint8_t), numbytes, f);
	fclose(f);
	

	// copy rom into buffer
	for (uint16_t i = 0; i < numbytes; ++i) {
		rom_buffer[START_ADDRESS + i] = tmp_buf[i];
	}

	free(tmp_buf);
	
    return 0;
}

/* ------------------ CPU Funcs */
void cpu_init(void) {
	c.SP = 0x0;
	c.PC = 0x0;
}
