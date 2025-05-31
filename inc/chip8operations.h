#ifndef CHIP8OPERATIONS_H
#define CHIP8OPERATIONS_H

#include "chip8.h"

// Initialize a chip8 structure with default values
chip8* initChip(void);

// Load Rom into chip memory
int load_rom(chip8* chip, const char* filename);

// Display the contents of registers, stack, and timers
void displayChipStatus(chip8* chip);

// Reads for key presses and updates registers accordingly
void handleInput(SDL_Event *event, chip8* chip);

// Correlate keys to index of chip keys
int mapKey(SDL_Keycode key);

#endif // CHIP8OPERATIONS_H