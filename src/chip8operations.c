#include "chip8operations.h"

BYTE fontset[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

chip8* initChip(void)
{
    // Allocate memory for the chip
    chip8* chip = malloc(sizeof(chip8));
    if (!chip) return NULL;

    // Initialize Chip values to zero
    for (int i = 0; i < 4096; i++) chip->memory[i] = 0;
    for (int i = 0; i < 16; i++) chip->V[i] = chip->stack[i] = chip->key[i] = 0;
    chip->I = chip->sp = chip->delay_timer = chip->sound_timer = chip->paused = chip->registerTarget = 0;
    chip->pc = 0x200;
    chip->end = 4095;

    // Initialize the graphics dispaly
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 32; j++)
            chip->gfx[i][j] = 0;
    
    // Load font set
    memcpy(&chip->memory[FONTSET_START], fontset, 80);

    return chip;
}

int load_rom(chip8* chip, const char* filename)
{
    FILE* rom = fopen(filename, "rb"); //Open the file in binary read mode
    if (rom == NULL) // If file is not found
    {
        fprintf(stderr, "Failed to open the file\n");
        return 0;
    }
    // Read the file into memory excluding the first 512 bytes
    chip->end = fread(&chip->memory[0x200], sizeof(BYTE), 4096-512, rom);
    fclose(rom);
    printf("Loaded %d Bytes into Chip Memory\n", chip->end);
    return 1;
}

void displayChipStatus(chip8* chip)
{
    time_t curtime;
    time(&curtime);
    // Print timestamp
    printf("%s ---------------------- \n", ctime(&curtime));
    // Memory and registry status
    printf("Program Counter Location: %u\n", chip->pc);
    printf("Next Opcode: 0x%x\n", chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1]);
    printf("Registers: \n\tV:     ");
    for (int i = 0; i < 16; i++)
        printf("%u ", chip->V[i]);
    // Stack Status
    printf("\n\tStack: ");
    for (int i = 0; i < 16; i++)
        printf("%u ", chip->stack[i]);
    // Index Location
    printf("\n\tIndex: %u\n", chip->I);
    // Chip Timers
    printf("Delay Timer: %u\nSound Timer: %u\n", chip->delay_timer, chip->sound_timer);
    // Chip Keys
    for (int i = 0; i < 16; i++)
        if (chip->key[i])
            printf("Key %d is being pushed!\n", i);
    printf("-------------------------------------------\n");
}

void handleInput(SDL_Event *event, chip8* chip)
{
    // If key is pressed or released
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) 
    {
        // Determine which key
        int chip8Key = mapKey(event->key.keysym.sym);
        // if the key is mapped then update its state
        if (chip8Key != -1)
        {
            if (event->type == SDL_KEYDOWN)
                chip->key[chip8Key] = 1;
            else if (event->type == SDL_KEYUP)
                chip->key[chip8Key] = 0;
        }
    }
}

int mapKey(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0xD;
        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_f: return 0xE;
        case SDLK_z: return 0xA;
        case SDLK_x: return 0x0;
        case SDLK_c: return 0xB;
        case SDLK_v: return 0xF;
        default: return -1; // Unmapped key
    }
}