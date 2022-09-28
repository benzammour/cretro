#include "ch8.h"

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
