#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "SDL/SDL.h"

typedef uint8_t BYTE;
typedef uint16_t WORD;

typedef struct chip8
{
    // Chip8 Memory
    BYTE memory[4096];
    // Chip8 Registers
    BYTE V[16];
    // Index Register
    WORD I;
    // Program Counter
    WORD pc;
    // Graphics Memory
    BYTE gfx[64][32];
    // Timers
    BYTE delay_timer;
    BYTE sound_timer;
    // Stack
    WORD stack[16];
    // Stack Pointer
    BYTE sp;
    // Keypad
    BYTE key[16];
    // ROM end indicator
    WORD end;
    // Chip Status
    bool paused;
    BYTE registerTarget;
} 
chip8;

#define FONTSET_START 0x50
#define TIMER_INTERVAL (1000 / 60) // 60 Hz (16.67ms per update)


#endif // CHIP8_H