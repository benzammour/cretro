#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "ch8.h"

struct CPU c;

uint8_t game_data[4096];

op_function instr_lookup[0xF + 1] = { [0 ... 0xF] = ILLEGAL_OPCODE };
op_function zero_prefixed_lookup[0xE + 1] = { [0 ... 0xE] = ILLEGAL_OPCODE };
op_function eight_prefixed_lookup[0xE + 1] = {  [0 ... 0xE] = ILLEGAL_OPCODE };
op_function e_prefixed_lookup[0xE + 1] = {  [0 ... 0xE] = ILLEGAL_OPCODE };
op_function f_prefixed_lookup[0x65 + 1] = {  [0 ... 0x65] = ILLEGAL_OPCODE };

#define GET_X(opcode) ((opcode >> 8) & 0x000Fu)
#define GET_Y(opcode) ((opcode >> 4) & 0x000Fu)
#define GET_N(opcode) (opcode & 0x000Fu)
#define GET_KK(opcode) (opcode & 0x00FFu)
#define GET_NNN(opcode) (opcode & 0x0FFFu)

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
		game_data[START_ADDRESS + i] = tmp_buf[i];
	}

	free(tmp_buf);

    return 0;
}


void ILLEGAL_OPCODE() {
	printf("Unallowed OP Code!\n");
}

void DEBUG() {
	printf("debug\n");
}

void ZERO_PREFIXED() {
	zero_prefixed_lookup[GET_N(c.opcode)]();
}

void EIGHT_PREFIXED() {
	eight_prefixed_lookup[GET_N(c.opcode)]();
}

void E_PREFIXED() {
	e_prefixed_lookup[GET_N(c.opcode)]();
}

void F_PREFIXED() {
	f_prefixed_lookup[GET_KK(c.opcode)]();
}

void print_0() {
	printf("0 prefixed\n");
}

void print_8() {
	printf("8 prefixed\n");
}

void print_e() {
	printf("e prefixed\n");
}

void print_f() {
	printf("f prefixed\n");
}

/* ------------------ CPU Funcs */
void cpu_init() {
	c.PC = START_ADDRESS;

	// Set up lookup table
	instr_lookup[0x0] = ZERO_PREFIXED;
	instr_lookup[0x1] = DEBUG;
	instr_lookup[0x2] = DEBUG;
	instr_lookup[0x3] = DEBUG;
	instr_lookup[0x4] = DEBUG;
	instr_lookup[0x5] = DEBUG;
	instr_lookup[0x6] = DEBUG;
	instr_lookup[0x7] = DEBUG;
	instr_lookup[0x8] = EIGHT_PREFIXED;
	instr_lookup[0x9] = DEBUG;
	instr_lookup[0xA] = DEBUG;
	instr_lookup[0xB] = DEBUG;
	instr_lookup[0xC] = DEBUG;
	instr_lookup[0xD] = DEBUG;
	instr_lookup[0xE] = E_PREFIXED;
	instr_lookup[0xF] = F_PREFIXED;

	zero_prefixed_lookup[0x0] = print_0;
	zero_prefixed_lookup[0xE] = print_0;

	eight_prefixed_lookup[0x0] = print_8;
	eight_prefixed_lookup[0x1] = print_8;
	eight_prefixed_lookup[0x2] = print_8;
	eight_prefixed_lookup[0x3] = print_8;
	eight_prefixed_lookup[0x4] = print_8;
	eight_prefixed_lookup[0x5] = print_8;
	eight_prefixed_lookup[0x6] = print_8;
	eight_prefixed_lookup[0x7] = print_8;
	eight_prefixed_lookup[0xE] = print_8;

	e_prefixed_lookup[0x1] = print_e;
	e_prefixed_lookup[0xE] = print_e;

	f_prefixed_lookup[0x07] = print_f;
	f_prefixed_lookup[0x0A] = print_f;
	f_prefixed_lookup[0x15] = print_f;
	f_prefixed_lookup[0x18] = print_f;
	f_prefixed_lookup[0x1E] = print_f;
	f_prefixed_lookup[0x29] = print_f;
	f_prefixed_lookup[0x33] = print_f;
	f_prefixed_lookup[0x55] = print_f;
	f_prefixed_lookup[0x65] = print_f;

}

uint8_t cpu_step() {
	// Get opcode and increment PC
	c.opcode = (game_data[c.PC] << 8u) | game_data[c.PC + 1];

	c.PC += 2;

	// Get and Execute Instruction 
	(*(instr_lookup[(c.opcode & 0xF000) >> 12]))();

	return 0;
}
