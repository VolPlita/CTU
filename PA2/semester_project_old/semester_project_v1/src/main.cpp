#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Game.hpp"

#define SCREEN_WIDTH 1536; // 1280 + 256
#define SCREEN_HEIGHT 832; // 640 + 96 * 2

#define TILE_WIDTH 64;
#define TILE_HEIGHT 32;
#define MAP_SIZE 20;
#define INIT_X SCREEN_WIDTH / 2 - 32;
#define INIT_Y TILE_HEIGHT + TILE_WIDTH; // 64 + 32
#define TILE_SPRITE_AMOUNT 20;
#define BUTTON_SPRITE_AMOUNT 8;

int main(int argc, char * argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cout << "Fail on initializing SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}