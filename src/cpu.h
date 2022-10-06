#ifndef CPU_H
#define CPU_H
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

// Clear the display
void OPC_00E0();

// RET: Return from a subroutine.
void OPC_00EE();

// JMP nnn: Jump to nnn
void OPC_1nnn();

// CALL nnn: call subroutine at nnn
void OPC_2nnn();

// SE Vx, byte: Skip next instruction if Vx = kk.
void OPC_3xkk();

// SNE Vx, byte: Skip next instruction if register Vx != kk.
void OPC_4xkk();

//  SE Vx, Vy: Skip next instruction if register Vx == Vy.
void OPC_5xy0();

// LD Vx, byte: Puts the value kk into register Vx
void OPC_6xkk();

// ADD Vx, byte: Adds the value kk to the value of register Vx, then stores the result in Vx
void OPC_7xkk();

// LD Vx, Vy: Stores the value of register Vy in register Vx.
void OPC_8xy0();

// OR Vx, Vy: Set Vx := Vx OR Vy
void OPC_8xy1();

// AND Vx, Vy: Set Vx := Vx AND Vy
void OPC_8xy2();

// XOR Vx, Vy: Set Vx := Vx XOR Vy
void OPC_8xy3();

// ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry
void OPC_8xy4();

// SUB Vx, Vy: Set Vx := Vx - Vy, set VF := NOT borrow.
void OPC_8xy5();

// SHR Vx: Set Vx = Vx SHR 1. 
void OPC_8xy6();

// SUBN Vx, Vy: Set Vx := Vy - Vx, set VF = NOT borrow.
void OPC_8xy7();

// SHL Vx: Set Vx = Vx SHL 1
void OPC_8xyE();

// SNE Vx, Vy Skip next instruction if Vx != Vy
void OPC_9xy0();

//  LD I, addr: Set I = nnn
void OPC_Annn();

// JP V0, addr: Jump to location nnn + V0
void OPC_Bnnn();

// RND Vx, byte: Set Vx = random byte AND kk
void OPC_Cxkk();

// DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void OPC_Dxyn();

// Ex9E - SKP Vx: Skip next instruction if key with the value of Vx is pressed.
void OPC_Ex9E();

// ExA1 - SKNP Vx: Skip next instruction if key with the value of Vx is not pressed.
void OPC_ExA1();

// Fx07 - LD Vx, DT: Set Vx = delay timer value. The value of DT is placed into Vx.
void OPC_Fx07();

// Fx0A - LD Vx, K: Wait for a key press, store the value of the key in Vx.
// All execution stops until a key is pressed, then the value of that key is stored in Vx.
void OPC_Fx0A();

// Fx15 - LD DT, Vx: Set delay timer = Vx. Delay Timer is set equal to the value of Vx.
void OPC_Fx15();

// Fx18 - LD ST, Vx: Set sound timer = Vx. Sound Timer is set equal to the value of Vx.
void OPC_Fx18();

// Fx1E - ADD I, Vx: Set I = I + Vx. The values of I and Vx are added, and the results are stored in I.
void OPC_Fx1E();

// Fx29 - LD F, Vx: Set I = location of sprite for digit Vx. The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
void OPC_Fx29();

// Fx33 - LD B, Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2.
void OPC_Fx33();

// Fx55 - LD [I], Vx: Stores V0 to VX in memory starting at address I. I is then set to I + x + 1.
void OPC_Fx55();

// Fx65 - LD Vx, [I]: Fills V0 to VX with values from memory starting at address I. I is then set to I + x + 1
void OPC_Fx65();


#endif
