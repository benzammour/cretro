#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "ch8.h"

struct CPU c;

uint8_t rom_buffer[4096];

op_function table[0xF + 1] = { NOOP };


void NOOP() {
}


void DEBUG() {
	printf("debug");
}


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
void cpu_init() {
	c.PC = START_ADDRESS;

	// Set up lookup table
	table[0x0] = DEBUG;
}

uint8_t cpu_step() {
	table[0x0]();

	return 0;
}


