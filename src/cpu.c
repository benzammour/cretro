#include <stdio.h>
#include "cpu.h"
#include "sound.h"

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
	
	sound_init();

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

	return 0;
}

// OP-Codes:
void OPC_00E0() {
	memset(m.video, 0, sizeof(m.video));
}

void OPC_00EE() {
	m.SP--;
	m.PC = m.stack[m.SP];
}

void OPC_1nnn() {
	uint16_t address = GET_NNN(m.opcode);

	m.PC = address;
}

void OPC_2nnn() {
	m.stack[m.SP] = m.PC;
	m.SP++;

	m.PC = GET_NNN(m.opcode);
}

void OPC_3xkk() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t byte = GET_KK(m.opcode);

	if (m.registers[Vx] == byte) {
		m.PC += 2;
	}
}

void OPC_4xkk() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t byte = GET_KK(m.opcode);

	if (m.registers[Vx] != byte) {
		m.PC += 2;
	}
}

void OPC_5xy0() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	if (m.registers[Vx] == m.registers[Vy]) {
		m.PC += 2;
	}
}

void OPC_6xkk() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t byte = GET_KK(m.opcode);

	m.registers[Vx] = byte;
}

void OPC_7xkk() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t byte = GET_KK(m.opcode);

	m.registers[Vx] += byte;
}

void OPC_8xy0() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	m.registers[Vx] = m.registers[Vy];
}

void OPC_8xy1() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	m.registers[Vx] |= m.registers[Vy];
}

void OPC_8xy2() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	m.registers[Vx] &= m.registers[Vy];
}

void OPC_8xy3() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	m.registers[Vx] ^= m.registers[Vy];
}

void OPC_8xy4() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	uint16_t sum = m.registers[Vx] + m.registers[Vy];

	m.registers[0xF] = (sum > 255) ? 1 : 0;

	m.registers[Vx] = sum & 0xFF;
}

void OPC_8xy5() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	m.registers[0xF] = (m.registers[Vx] > m.registers[Vy]) ? 1 : 0;

	m.registers[Vx] -= m.registers[Vy];
}

void OPC_8xy6() {
	uint8_t Vx = GET_X(m.opcode);

	// Save LSB in VF
	m.registers[0xF] = (m.registers[Vx] & 0x1);

	m.registers[Vx] = m.registers[Vx] >> 1;
}

void OPC_8xy7() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	m.registers[0xF] = (m.registers[Vy] > m.registers[Vx]) ? 1 : 0;

	m.registers[Vx] = m.registers[Vy] - m.registers[Vx];
}

void OPC_8xyE() {
	uint8_t Vx = GET_X(m.opcode);

	// Save MSB in VF because if msb = 1 set VF to 1
	// analogously with msb = 0
	m.registers[0xF] = (m.registers[Vx] & 0b10000000) >> 7;

	m.registers[Vx] = m.registers[Vx] << 1;
}

void OPC_9xy0() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t Vy = GET_Y(m.opcode);

	if (m.registers[Vx] != m.registers[Vy]) {
		m.PC += 2;
	}
}

void OPC_Annn() {
	uint16_t reg_addr = GET_NNN(m.opcode);

	m.index = reg_addr;
}

void OPC_Bnnn() {
	uint16_t reg_addr = GET_NNN(m.opcode);
	uint8_t offset = m.registers[0];
	
	m.PC = reg_addr + offset;
}

void OPC_Cxkk() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t byte = GET_KK(m.opcode);
	uint8_t random = rand() & byte;
	
	m.registers[Vx] = random & byte;
}

void OPC_Dxyn() {
	uint8_t Vx = (m.opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (m.opcode & 0x00F0u) >> 4u;
	uint8_t height = m.opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t vxValue = m.registers[Vx] % VIDEO_WIDTH;
	uint8_t vyValue = m.registers[Vy] % VIDEO_HEIGHT;

	m.registers[0xF] = 0;

	for (uint8_t row = 0; row < height; row++) {
		uint8_t spritePixel = m.memory[m.index + row];

		for (uint8_t col = 0; col < 8; col++) {
			uint8_t bit = spritePixel & (0b10000000 >> col);
			uint32_t* pixel = &m.video[(vyValue + row) * VIDEO_WIDTH + (vxValue + col)];

			// Sprite pixel is set
			if (bit) {
				if (*pixel == 0xFFFFFFFF) {	// pixel on screen => set carry bit
					m.registers[0xF] = 1;
				}

				*pixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void OPC_Ex9E() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t key = m.registers[Vx];

	if (m.keys[key]) {
		m.PC += 2;
	}
}

void OPC_ExA1() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t key = m.registers[Vx];

	if (!m.keys[key]) {
		m.PC += 2;
	}
}

void OPC_Fx07() {
	uint8_t Vx = GET_X(m.opcode);
	m.registers[Vx] = m.delay_timer;
}

void OPC_Fx0A() {
	uint8_t Vx = GET_X(m.opcode);

	for (int i = 0; i < 16; i++) {
		if (m.keys[i]) {
			m.registers[Vx] = i;
			return;
		}
	}

	m.PC -= 2;
}

void OPC_Fx15() {
	uint8_t Vx = GET_X(m.opcode);

	m.delay_timer = m.registers[Vx];
}

void OPC_Fx18() {
	uint8_t Vx = GET_X(m.opcode);

	m.sound_timer = m.registers[Vx];
}

void OPC_Fx1E() {
	uint8_t Vx = GET_X(m.opcode);

	m.index += m.registers[Vx];
}

void OPC_Fx29() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t value = m.registers[Vx];

	m.index = FONTSET_START_ADDRESS + (5 * value); // 5 = font char width
}

void OPC_Fx33() {
	uint8_t Vx = GET_X(m.opcode);
	uint8_t value = m.registers[Vx];

	// 1s
	m.memory[m.index + 2] = value % 10;
	value /= 10;

	// 10s
	m.memory[m.index + 1] = value % 10;
	value /= 10;

	// 100s
	m.memory[m.index] = value % 10;
}

void OPC_Fx55() {
	uint8_t Vx = GET_X(m.opcode);

	for (uint8_t i = 0; i <= Vx; i++) {
		m.memory[m.index + i] = m.registers[i];
	}
}

void OPC_Fx65() {
	uint8_t Vx = GET_X(m.opcode);

	for (uint8_t i = 0; i <= Vx; i++) {
		m.registers[i] = m.memory[m.index + i];
	}
}
