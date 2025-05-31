#include "../inc/chip8emulator.h"
#include <windows.h>


// Make command: gcc -Iinc -Iinc/SDL src/main.c src/chip8operations.c src/opcode.c -Llib -lSDL2main -lSDL2 -o emulator.ex

void renderGFX(chip8* chip, SDL_Renderer* renderer);

int main(int argc, char* argv[])
{
    char buff[100];
    printf("Program is Running...\n");
    // Initialize Chip8
    chip8* myChip8 = initChip();
    if (!myChip8)
    {
        printf("Could not allocate memory for chip!\n");
        return 1;
    }
    printf("Chip8 Initialization Successfull!\n");

    printf("Please type ROM path relative to executable: ");
    scanf("%s", buff);

    // Load target ROM (File path relative to that of emulator.exe)
    if (!load_rom(myChip8, buff))
    {
        printf("Could not load Rom!\n");
        free(myChip8);
        return -1;
    }
    
    // Error Checking
    printf("Chip end byte in memory location: %d\n", myChip8->end);

    // SDL Initialization
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        free(myChip8);
        return -2;
    }

    // Timed execution for testing
    Uint32 start = SDL_GetTicks();
    Uint32 cur_time, lastTimerUpdate;
    lastTimerUpdate = SDL_GetTicks();

    SDL_Window* window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        free(myChip8);
        return -2;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        free(myChip8);
        return -2;
    }

    SDL_Event event;

    bool running = true;

    while (running)
    {
        
        // Decrementing timers
        cur_time = SDL_GetTicks();
        if (cur_time - lastTimerUpdate >= TIMER_INTERVAL)
        {
            if (myChip8->delay_timer > 0) myChip8->delay_timer--;
            if (myChip8->sound_timer > 0)
            {
                myChip8->sound_timer--;
                if (myChip8->sound_timer == 0) printf("Stopped beeping\n");
            }
            lastTimerUpdate = cur_time;
        }
        
        // Check for user input
        while(SDL_PollEvent(&event))
        {
            handleInput(&event, myChip8);
            if (event.type == SDL_QUIT) 
            {
                printf("Exiting Emulator...\n");
                running = false;
                break;
            }  
        }
        
        // If the chip is waiting for a key press
        if (myChip8->paused)
        {
            getKeyPress(myChip8, myChip8->registerTarget);
            if (myChip8->paused) continue;
        }

        // Fetch opcode from memory and execute
        handle_opcode(myChip8, fetch_opcode(myChip8));

        //displayChipStatus(myChip8);
        for (int i = 0; i < 16; i++)
        {
            if (myChip8->key[i])
                printf("Key %d is being pressed!\n", i);
        }

        renderGFX(myChip8, renderer);
        SDL_Delay(1);
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Free allocated memory
    free(myChip8);

    printf("Program has terminated!\n");
    return 0;
}

void renderGFX(chip8* chip, SDL_Renderer* renderer)
{
    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Iterate through gfx drawing large square pixels on display
    for (int x = 0; x < 64; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            if (chip->gfx[x][y] == 1)
            {
                SDL_Rect rect = {
                    .x = x * 10,
                    .y = y * 10,
                    .w = 10,
                    .h = 10
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    
    // Update Screen
    SDL_RenderPresent(renderer);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}