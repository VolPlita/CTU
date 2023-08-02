#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <iostream>

#include "GameControll.hpp"
#include "Buildings.hpp"

using namespace std;

int main(int argc, char * args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        cout << "Failed to initialize SDL. SDL_ERROR:" << SDL_GetError() << endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;

    CRenderWindow window("GAME v0.0", 1280, 720);
    SDL_Texture * house_tex = window.loadTexture("images/house.png");

    CHouse house1(100, 100, house_tex);

    bool gameRunning = true;
    SDL_Event event;
    while (gameRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                gameRunning = false;
        }
        window.clearScreen();
        window.render(house1);
        window.display();
    }
    SDL_Quit();

    return EXIT_SUCCESS;
}