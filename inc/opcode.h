#ifndef OPCODE_H
#define OPCODE_H

#include "chip8.h"

// Read opcode from memory
WORD fetch_opcode(chip8* chip);

// Seperate opcode into 4 bits for processing
BYTE* sep_hexa(WORD opcode);

// Given an opcode, execute the corresponding function
void handle_opcode(chip8* chip8, WORD opcode);

// Opcode Specific handling functions
/*
 * X and Y represents a register i.e. V[X]
 * N represents an whole number value
 * Descriptions of functions in chip8.c
 */

// Call machine code routine (Not necessary)
void callMachineCode(chip8* chip, WORD address);

// Clear Display
void clearDisplay(chip8* chip);

// Return from subroutine to address from stack
void returnSubroutine(chip8* chip);

// Jump to a different address
void jumpAddress(chip8* chip, WORD address);

// Call a subroutine at a specific address
void callSubroutine(chip8* chip, WORD addresss);

// Skip next instruction if VX equal N
void skipWithEqualInt(chip8* chip, BYTE X, BYTE N);

// Skip next instruction if VX does not equal N
void skipWithNonequalInt(chip8* chip, BYTE X, BYTE N);

// Skip next instruction if VX equals VY
void skipWithRegisters(chip8* chip, BYTE X, BYTE Y);

// Set VX equal to N
void setRegisterToInt(chip8* chip, BYTE X, BYTE N);

// Set VX equal to N plus VX
void addToRegister(chip8* chip, BYTE X, BYTE N);

// Set VX equal to VY
void setRegistertoRegister(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VX | VY
void orRegisters(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VX & VY
void andRegisters(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VX ^ VY
void xorRegisters(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VX + VY
void addRegisters(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VX - VY
void subtractRegisters(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VX >> 1
void rshiftRegister(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VY - VX
void revSubtractRegisters(chip8* chip, BYTE X, BYTE Y);

// Set VX to be equal to VX << 1
void lshiftRegister(chip8* chip, BYTE X, BYTE Y);

// Skip next instruction if VX does not equal VY
void skipWithDiffRegisters(chip8* chip, BYTE X, BYTE Y);

// Set I to be equal to address
void setIndex(chip8* chip, WORD address);

// Jump to address + V0
void jumpAddressWithV0(chip8* chip, WORD address);

// Set VX to be equal to random number (0-255) & V0
void setRegRandom(chip8* chip, BYTE X, BYTE N);

// Display a sprite at (VX, VY) with width 8 pixels and height N
void displaySprite (chip8* chip, BYTE X, BYTE Y, BYTE N);

// Skip next instruction if key stored in VX is pressed
void ifKeyPress(chip8* chip, BYTE X);

// Skip next instruction if key stored in VX is not pressed
void ifNotKeyPress(chip8* chip, BYTE X);

// Set VX to be equal to the delay timer
void getDelayTimer(chip8* chip, BYTE X);

// Wait for a key press then store in VX
void getKeyPress(chip8* chip, BYTE X);

// Set delay timer to be equal to VX
void setDelayTimer(chip8* chip, BYTE X);

// Set sound timer to be equal to VX
void setSoundTimer(chip8* chip, BYTE X);

// Increment Index by VX
void incrementIndex(chip8* chip, BYTE X);

// Set Index to be the location of the sprite for the charcter in VX
void setIndexToSprite(chip8* chip, BYTE X);

// Store the binary-coded decimal representaion of VX (start with hundreds in I)
void setVXAsBCD(chip8* chip, BYTE X);

// Store from V0 to VX  in memory starting at address I
void dumpRegisters(chip8* chip, BYTE X);

// Fill from V0 to VX with memory values starting at address I
void loadRegisters(chip8* chip, BYTE X);

// OPCODE ASSISTANCE FUNCTIONS
int findKeyPress(chip8* chip);

#endif // OPCODE_H