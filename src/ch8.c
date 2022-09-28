#include <stdio.h>
#include "cpu_opcodes.h"
#include "ch8.h"

op_function instr_lookup[0xF + 1] = { [0 ... 0xF] = ILLEGAL_OPCODE };
op_function zero_prefixed_lookup[0xE + 1] = { [0 ... 0xE] = ILLEGAL_OPCODE };
op_function eight_prefixed_lookup[0xE + 1] = { [0 ... 0xE] = ILLEGAL_OPCODE };
op_function e_prefixed_lookup[0xE + 1] = { [0 ... 0xE] = ILLEGAL_OPCODE };
op_function f_prefixed_lookup[0x65 + 1] = { [0 ... 0x65] = ILLEGAL_OPCODE };

uint8_t fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct machine_t m = { 
	{ [0 ... (4096-1)] = 0 }, 
	{ [0 ... (REGISTER_COUNT-1)] = 0 }, 
	{ [0 ... (STACK_LEVELS-1)] = 0 }, 
	{ [0 ... (SCREEN_DIMENSIONS-1)] = 0 }, 
	{ [0 ... (KEY_SIZE-1)] = 0 }, 
	0, 
	0,
	0,
	0,
	0,
	0,
	0
};

uint8_t rom_load(const char *filename) {
    FILE *f;
	uint8_t *tmpBuffer;
	uint16_t romSize;

	// Open ROM
    f = fopen(filename, "r");
	if(f == NULL) {
		return 1;
	}

	// Get the number of bytes
	fseek(f, 0L, SEEK_END);
	romSize = ftell(f);
	
	// reset pointer to beginning of file
	fseek(f, 0L, SEEK_SET);	
	
	// grab sufficient memory for the buffer 
	tmpBuffer = (uint8_t*) calloc(romSize, sizeof(uint8_t));	
	if(tmpBuffer == NULL) {
		fclose(f);
		return 1;
	}

	fread(tmpBuffer, sizeof(uint8_t), romSize, f);
	fclose(f);
	
	// copy rom into buffer
	for (uint16_t i = 0; i < romSize; i++) {
		m.memory[START_ADDRESS + i] = tmpBuffer[i];
	}

	free(tmpBuffer);

    return 0;
}

void ILLEGAL_OPCODE() {
	printf("Unallowed OP Code!\n");
}

void ZERO_PREFIXED() {
	zero_prefixed_lookup[GET_N(m.opcode)]();
}

void EIGHT_PREFIXED() {
	eight_prefixed_lookup[GET_N(m.opcode)]();
}

void E_PREFIXED() {
	e_prefixed_lookup[GET_N(m.opcode)]();
}

void F_PREFIXED() {
	f_prefixed_lookup[GET_KK(m.opcode)]();
}

void machine_init() {
	m.PC = START_ADDRESS;
	srand(time(NULL));
	
	for (int i = 0; i < FONTSET_SIZE; i++) {
        m.memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

	// Set up lookup table
	instr_lookup[0x0] = ZERO_PREFIXED;
	instr_lookup[0x1] = OPC_1nnn;
	instr_lookup[0x2] = OPC_2nnn;
	instr_lookup[0x3] = OPC_3xkk;
	instr_lookup[0x4] = OPC_4xkk;
	instr_lookup[0x5] = OPC_5xy0;
	instr_lookup[0x6] = OPC_6xkk;
	instr_lookup[0x7] = OPC_7xkk;
	instr_lookup[0x8] = EIGHT_PREFIXED;
	instr_lookup[0x9] = OPC_9xy0;
	instr_lookup[0xA] = OPC_Annn;
	instr_lookup[0xB] = OPC_Bnnn;
	instr_lookup[0xC] = OPC_Cxkk;
	instr_lookup[0xD] = OPC_Dxyn;
	instr_lookup[0xE] = E_PREFIXED;
	instr_lookup[0xF] = F_PREFIXED;

	zero_prefixed_lookup[0x0] = OPC_00E0;
	zero_prefixed_lookup[0xE] = OPC_00EE;

	eight_prefixed_lookup[0x0] = OPC_8xy0;
	eight_prefixed_lookup[0x1] = OPC_8xy1;
	eight_prefixed_lookup[0x2] = OPC_8xy2;
	eight_prefixed_lookup[0x3] = OPC_8xy3;
	eight_prefixed_lookup[0x4] = OPC_8xy4;
	eight_prefixed_lookup[0x5] = OPC_8xy5;
	eight_prefixed_lookup[0x6] = OPC_8xy6;
	eight_prefixed_lookup[0x7] = OPC_8xy7;
	eight_prefixed_lookup[0xE] = OPC_8xyE;

	e_prefixed_lookup[0x1] = OPC_ExA1;
	e_prefixed_lookup[0xE] = OPC_Ex9E;

	f_prefixed_lookup[0x07] = OPC_Fx07;
	f_prefixed_lookup[0x0A] = OPC_Fx0A;
	f_prefixed_lookup[0x15] = OPC_Fx15;
	f_prefixed_lookup[0x18] = OPC_Fx18;
	f_prefixed_lookup[0x1E] = OPC_Fx1E;
	f_prefixed_lookup[0x29] = OPC_Fx29;
	f_prefixed_lookup[0x33] = OPC_Fx33;
	f_prefixed_lookup[0x55] = OPC_Fx55;
	f_prefixed_lookup[0x65] = OPC_Fx65;

}

uint8_t cpu_step() {
	// Get opcode and increment PC
	m.opcode = (m.memory[m.PC] << 8) | m.memory[m.PC + 1];

	m.PC += 2;

	// Get and Execute Instruction 
	(*(instr_lookup[(m.opcode & 0xF000) >> 12]))();

	// Decrease timers if necessary
	if (m.delay_timer > 0) {
		m.delay_timer--;
	}

	if (m.sound_timer > 0) {
		m.sound_timer--;
	}

	return 0;
}
