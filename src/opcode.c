#include "opcode.h"

WORD fetch_opcode(chip8* chip)
{
    // Wrap the counter around if necessary
    if (chip->pc > chip->end+510)
        chip->pc = 512;
    // Combine the two bytes at the program counter to form the opcode
    WORD opcode = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
    // Move to the next opcode
    chip->pc += 2;
    return opcode;
}

BYTE* sep_hexa(WORD opcode)
{
    // Create an array to store the 4 hex digits
    static BYTE hexa[4];
    // Use bitwise operations to separate the opcode
    hexa[0] = (opcode & 0xF000) >> 12;
    hexa[1] = (opcode & 0x0F00) >> 8;
    hexa[2] = (opcode & 0x00F0) >> 4;
    hexa[3] = opcode & 0x000F;
    return hexa;
}

void handle_opcode(chip8* chip, WORD opcode)
{
    // Separate the opcode into 4 hex digits
    BYTE* hexa = sep_hexa(opcode);
    if (hexa[0] == 0x0F) //If the highest 4 bits of the opcode is F |Not Finished|
    {
        switch ((hexa[2] << 4) | hexa[3]) // Check the last byte of the opcode
        {
            case 0x07: // FX07
                getDelayTimer(chip, hexa[1]);
                break;
            case 0x0A: // FX0A
                getKeyPress(chip, hexa[1]);
                break;
            case 0x15: // FX15
                setDelayTimer(chip, hexa[1]);
                break;
            case 0x18: // FX18
                setSoundTimer(chip, hexa[1]);
                break;
            case 0x1E: // FX1E
                incrementIndex(chip, hexa[1]); 
                break;
            case 0x29: // FX29
                setIndexToSprite(chip, hexa[1]);
                break;
            case 0x33: // FX33
                setVXAsBCD(chip, hexa[1]);
                break;
            case 0x55: // FX55
                dumpRegisters(chip, hexa[1]);
                break;
            case 0x65: // FX65
                loadRegisters(chip, hexa[1]);
                break;
            default:
                printf("Unknown opcode\n");
                break;
        }
    }
    else if (hexa[0] == 0x0E) // Opcodes identifiable by the last byte and first hex digit
    {
        switch((hexa[2] << 4) | hexa[3])
        {
            case 0x9E: // EX9E
                ifKeyPress(chip, hexa[1]);
                break;
            case 0xA1: // EXA1
                ifNotKeyPress(chip, hexa[1]);
                break;
            default:
                printf("Unknown opcode\n");
                break;
        }
    }
    else if (hexa[0] == 0x08) // Opcodes identifiable by the last hex digit and first hex digit
    {
        switch(hexa[3])
        {
            case 0x00: // 8XY0
                setRegistertoRegister(chip, hexa[1], hexa[2]);
                break;
            case 0x01: // 8XY1
                orRegisters(chip, hexa[1], hexa[2]);
                break;
            case 0x02: // 8XY2
                andRegisters(chip, hexa[1], hexa[2]);
                break;
            case 0x03: // 8XY3
                xorRegisters(chip, hexa[1], hexa[2]);
                break;
            case 0x04: //8XY4
                addRegisters(chip, hexa[1], hexa[2]);
                break;
            case 0x05: //8XY5
                subtractRegisters(chip, hexa[1], hexa[2]);
                break;
            case 0x06: //8XY6
                rshiftRegister(chip, hexa[1], hexa[2]);
                break;
            case 0x07: // 8XY7
                revSubtractRegisters(chip, hexa[1], hexa[2]);
                break;
            case 0x0E: //8XYE
                lshiftRegister(chip, hexa[1], hexa[2]);
                break;
            default:
                printf("Unknown opcode\n");
                break;
        }
    }
    else if (hexa[0] == 0x00) // Opcodes starting with 0x00
    {
        switch ((hexa[2] << 4) | hexa[3]) 
        {
            case 0xE0: // 00E0
                clearDisplay(chip);
                break;
            case 0xEE: // 00EE
                returnSubroutine(chip);
                break;
            default: // 0NNN
                callMachineCode(chip, opcode & 0x0FFF);
                break;
        }
    }   
    else{
        switch (hexa[0]) // Handle opcodes identifiable by first hex digit alone
        {
            case 0x01: // 1NNN
                jumpAddress(chip, opcode & 0x0FFF);
                break;
            case 0x02: // 2NNN
                callSubroutine(chip, opcode & 0x0FFF);
                break;
            case 0x03: // 3XNN
                skipWithEqualInt(chip, hexa[1], (BYTE) opcode & 0x00FF);
                break;
            case 0x04: // 4XNN
                skipWithNonequalInt(chip, hexa[1], (BYTE) opcode & 0x00FF);
                break;
            case 0x05: // 5XY0 
                if (hexa[3] == 0x00)
                    skipWithRegisters(chip, hexa[1], hexa[2]);
                else
                    printf("Unknown Opcode!\n");
                break;
            case 0x06: // 6XNN
                setRegisterToInt(chip, hexa[1], (BYTE) opcode & 0x00FF);
                break;
            case 0x07: // 7XNN
                addToRegister(chip, hexa[1], (BYTE) opcode & 0x00FF);
                break;
            case 0x09: // 9XY0
                if (hexa[3] == 0x00)
                    skipWithDiffRegisters(chip, hexa[1], hexa[2]);
                else
                    printf("Unknown Opcode!\n");
                break;
            case 0x0A: // ANNN
                setIndex(chip, opcode & 0x0FFF);
                break;
            case 0x0B: // BNNN
                jumpAddress(chip, opcode & 0x0FFF);
                break;
            case 0x0C: // CXNN
                setRegRandom(chip, hexa[1], (BYTE) opcode & 0x00FF);
                break;
            case 0x0D: // DXYN
                displaySprite(chip, hexa[1], hexa[2], hexa[3]);
                break;
            default:
                printf("Unknown opcode\n");
                break;
        }
    }
}

// Call machine code routine (Not necessary)
void callMachineCode(chip8* chip, WORD address)
{
    printf("Call RCA 1802 program at address %hx\n", address);
}

// Clear Display
void clearDisplay(chip8* chip)
{
    printf("Clear screen\n");
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 32; j++)
            chip->gfx[i][j] = 0;
}

//  Return from a subroutine
void returnSubroutine(chip8* chip)
{
    printf("Return from subroutine to address %hx\n", chip->stack[chip->sp - 1]);
    // Set the program counter to the last address called on stack frame
    chip->pc = chip->stack[--chip->sp];
}

// Jump to a different address
void jumpAddress(chip8* chip, WORD address)
{
    printf("Jump to %X\n", address);
    chip->pc = address;
}

// Call a subroutine at address
void callSubroutine(chip8* chip, WORD address)
{
    printf("Call subroutine at %X\n", address);
    // Save the next opcode for after the subroutine in the stack
    chip->stack[chip->sp++] = chip->pc;
    chip->pc = address;
}

// Skip next instruction if VX equals N
void skipWithEqualInt(chip8* chip, BYTE X, BYTE N)
{
    printf("Skip next instruction if V%X == %X\n", X, N);
    if (chip->V[X] == N)
    {
        printf("Skip!\n");
        chip->pc += 0x0002;
    }
}

// Skip next instruction if VX does not equal N
void skipWithNonequalInt(chip8* chip, BYTE X, BYTE N)
{
    printf("Skip next instruction if V%X != %X\n", X, N);
    if (chip->V[X] != N)
    {
        printf("Skip!\n");
        chip->pc += 0x0002;
    }      
}

// Skip next instruction if VX equals VY
void skipWithRegisters(chip8* chip, BYTE X, BYTE Y)
{
    printf("Skip next instruction if V%X = V%X\n", X, Y);
    if (chip->V[X] == chip->V[Y])
    {
        printf("Skip!\n");
        chip->pc += 0x0002;
    }
}

// Set VX equal to N
void setRegisterToInt(chip8* chip, BYTE X, BYTE N)
{
    printf("Set V%X to %X\n", X, N);
    chip->V[X] = N;
}

// Set VX equal to N plus VX
void addToRegister(chip8* chip, BYTE X, BYTE N)
{
    printf("Add %X to V%X\n", N, X);
    WORD sum = chip->V[X] + N;
    chip->V[X] = (BYTE) sum & 0xFF;
}

// Set VX equal to VY
void setRegistertoRegister(chip8* chip, BYTE X, BYTE Y)
{
    printf("Set V%X to V%X\n", X, Y);
    chip->V[X] = chip->V[Y];
}

// Set VX to be equal to VX | VY
void orRegisters(chip8* chip, BYTE X, BYTE Y)
{

    printf("Set V%X to V%X OR V%X\n", X, X, Y);
    if (X == 0xF)
        return;
    chip->V[X] |= chip->V[Y];

    // Quirks Test
    chip->V[0xF] = 0;
}

// Set VX to be equal to VX & VY
void andRegisters(chip8* chip, BYTE X, BYTE Y)
{
    printf("Set V%X to V%X AND V%X\n", X, X, Y);
    if (X == 0xF)
        return;
    chip->V[X] &= chip->V[Y];

    // Quirks Test
    chip->V[0xF] = 0;
}

// Set VX to be equal to VX ^ VY
void xorRegisters(chip8* chip, BYTE X, BYTE Y)
{
    printf("Set V%X to V%X XOR V%X\n", X, X, Y);
    if (X == 0xF)
        return;
    chip->V[X] ^= chip->V[Y];

    // Quirks Test
    chip->V[0xF] = 0;
}

// Set VX to be equal to VX + VY
void addRegisters(chip8* chip, BYTE X, BYTE Y)
{
    printf("Add V%X to V%X\n", X, Y);
    // Compute the sum weary of potential overflow
    WORD sum = chip->V[X] + chip->V[Y];
    // If there is overflow throw VF flag
    // Only keep lowest 8 bits
    chip->V[X] = (BYTE) sum & 0xFF;
    chip->V[0xF] = (sum > 255) ? 1 : 0;
}

// Set VX to be equal to VX - VY
void subtractRegisters(chip8* chip, BYTE X, BYTE Y)
{
    printf("Subtract V%X from V%X\n", X, Y);
    // Check for underflow
    BYTE diff = chip->V[X] - chip->V[Y];
    BYTE org = chip->V[X];
    chip->V[X] = diff;
    chip->V[0xF] = (org >= chip->V[Y]) ? 1 : 0;
}

// Set VX to be equal to VX >> 1
void rshiftRegister(chip8* chip, BYTE X, BYTE Y)
{
    printf("Shift V%X right\n", X);
    BYTE hold = chip->V[X];
    chip->V[X] >>= 1;
    chip->V[0xF] = hold % 2;
}

// Set VX to be equal to VY - VX
void revSubtractRegisters(chip8* chip, BYTE X, BYTE Y)
{
    printf("Set V%X to V%X - V%X\n", X, Y, X);
    BYTE hold = chip->V[X];
    chip->V[X] = chip->V[Y] - chip->V[X];
    chip->V[0xF] = (chip->V[Y] >= hold) ? 1 : 0;
}

// Set VX to be equal to VX << 1
void lshiftRegister(chip8* chip, BYTE X, BYTE Y)
{
    printf("Shift V%X left\n", X);
    BYTE hold = chip->V[X];
    chip->V[X] <<= 1;
    // And operation with 100000000 to isolate MSB
    chip->V[0xF] = (hold & 0x80) >> 7;
}

// Skip next instruction if VX does not equal VY
void skipWithDiffRegisters(chip8* chip, BYTE X, BYTE Y)
{
    printf("Skip next instruction if V%X != V%X\n", X, Y);
    if (chip->V[X] != chip->V[Y])
    {
        printf("Skip!\n");
        chip-> pc += 0x0002;
    }
}

// Set I to be equal to address
void setIndex(chip8* chip, WORD address)
{
    printf("Set index to %X\n", address);
    chip->I = address;
}

// Jump to address + V0
void jumpAddressWithV0(chip8* chip, WORD address)
{
    printf("Jump to %X + V0\n", address);
    chip->pc = ((WORD) chip->V[0]) + address;
}

// Set VX to be equal to random number (0-255) & V0
void setRegRandom(chip8* chip, BYTE X, BYTE N)
{
    printf("Set V%X to random number AND %X\n", X, N);
    chip->V[X] = ((BYTE) rand() % 256) & (N);         
}

// Display a sprite at (VX, VY) with width 8 pixels and height N
void displaySprite (chip8* chip, BYTE X, BYTE Y, BYTE N)
{
    BYTE spriteRow, spritePixel, xpos, ypos;
    chip->V[0xF] = 0;
    printf("Draw sprite at V%X, V%X with height %X\n", X, Y, N);
    for (int i = 0; i < N; i++)
    {
        // Retrieve the sprite byte for row
        spriteRow = chip->memory[chip->I + i];
        for (int j = 0; j < 8; j++)
        {
            // Determine if current byte is already on
            spritePixel = (spriteRow >> (7-j)) & 1;
            // Determine display position with wrapping
            xpos = (chip->V[X] + j) % 64;
            ypos = (chip->V[Y] + i) % 32;

            // Check for collision
            if (spritePixel)
            {
                if (chip->gfx[xpos][ypos])
                    chip->V[0xF] = 1;
                chip->gfx[xpos][ypos] ^= 1;
            }
        }
    }
}

// Skip next instruction if key stored in VX is pressed
void ifKeyPress(chip8* chip, BYTE X)
{
    printf("Skip next instruction if key in register %X is pressed\n", X);
    if (chip->key[chip->V[X] & 0x0F])
    {
        printf("Skip!\n");
        chip->pc += 0x0002;
    }
}

// Skip next instruction if key stored in VX is not pressed
void ifNotKeyPress(chip8* chip, BYTE X)
{
    printf("Skip next instruction if key in register %X is not pressed\n", X);
    if (!chip->key[chip->V[X] & 0x0F])
    {
        printf("Skip!\n");
        chip->pc += 0x0002;
    }
}

// Set VX to be equal to the delay timer
void getDelayTimer(chip8* chip, BYTE X)
{
    printf("Delay timer to register %X\n", X);
    chip->V[X] = chip->delay_timer;
}

// Wait for a key press then store in VX
// This opcode has special implementation within the main loop of the emulator
void getKeyPress(chip8* chip, BYTE X)
{
    int key = findKeyPress(chip);
    if (key != -1)
    {
        chip->V[X] = key;
        chip->paused = 0;
        return;
    }
    chip->registerTarget = X;
    chip->paused = 1;
}

// Set delay timer to be equal to VX
void setDelayTimer(chip8* chip, BYTE X)
{
    printf("Set delay timer to register %X\n", X);
    chip->delay_timer = chip->V[X];
}

void setSoundTimer(chip8* chip, BYTE X)
{
    printf("Set sound timer to register %X\n", X);
    chip->sound_timer = chip->V[X];
}

// Increment Index by VX
void incrementIndex(chip8* chip, BYTE X)
{
    printf("Add to index register %X\n", X);
    chip->I += chip->V[X];            
}

// Set Index to be the location of the sprite for the charcter in VX
void setIndexToSprite(chip8* chip, BYTE X)
{
    printf("Set index to sprite %X\n", X);
    chip->I = (WORD) FONTSET_START + (chip->V[X] * 5);
}

// Store the binary-coded decimal representaion of VX (start with hundreds in I)
void setVXAsBCD(chip8* chip, BYTE X)
{
    printf("Store BCD of %X\n", X);
    BYTE bcdvx = chip->V[X]; 
    chip->memory[chip->I + 2] = bcdvx % 10; // Ones place
    chip->memory[chip->I + 1] = (bcdvx / 10) % 10; // Tens place
    chip->memory[chip->I] = bcdvx / 100; // Hundreds place     
}

// Store from V0 to VX  in memory starting at address I
void dumpRegisters(chip8* chip, BYTE X)
{
    printf("Store registers V0 to V%X\n", X);
    for (int i = 0; i <= X; i++)
    {
        chip->memory[chip->I + i] = chip->V[i];
    }
    
    // Quirks Test 
    chip->I += (X+1);
}

// Fill from V0 to VX with memory values starting at address I
void loadRegisters(chip8* chip, BYTE X)
{
    printf("Load registers V0 to V%X\n", X);
    for (int i = 0; i <= X; i++)
    {
        chip->V[i] = chip->memory[chip->I + i];
    }

    // Quirks Test 
    chip->I += (X+1);
}

// OPCODE ASSISTANCE FUNCTIONS

// Iterate through key array to find presses
int findKeyPress(chip8* chip)
{
    for (int i = 0; i < 16; i++)
    {
        if (chip->key[i] == 1)
        {
            return i;
        }
    }
    return -1;
}