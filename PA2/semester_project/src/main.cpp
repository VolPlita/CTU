/**
 * @file main.cpp
 * @brief Entry point of the program
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <filesystem>

#include "define.hpp"
#include "Menu.hpp"
#include "Parser.hpp"
#include "Game.hpp"


/**
 * @brief Initializes SDL_Window with title defined in "define.hpp" and SDL_Renderer
 * @param win Pointer to the window pointer
 * @param ren Reference to the renderer pointer
 */
void init_SDL(SDL_Window * win, SDL_Renderer *& ren)
{
    // Create a window and renderer
    win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // Set the renderer draw color
    SDL_SetRenderDrawColor(ren, 0x66, 0x66, 0xBB, 0xFF);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief The main function of the program. 
 * Contains main loop with event polling. 
 * Instance of CGame is constructed here as well aas two instances of CMenu which are used for initial and world loading menus
 * @param argc Number of command-line arguments
 * @param argv Array of command-line arguments
 * @return Exit status
 */
int main(int argc, char * argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cout << "Fail on initializing SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }
    if (TTF_Init() == -1)
    {
        std::cout << "Failed to initialize fonts: " << SDL_GetError() << std::endl;
//        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    init_SDL(window, renderer);

    CMenu chooseMenu; // chooseMenu does not have any options when created, it needs to be updated when it is needed to be used

    CMenu initMenu;
    initMenu.add_option(renderer, "New game");
    initMenu.add_option(renderer, "Load game");
    initMenu.add_option(renderer, "Exit");

    CGame G("src/images/textures.bmp", "src/images/buttons.bmp", renderer);  
    G.add_flood(1000 * 120); // Repeat each two minutes
    G.add_treegrow(1000 * 60); // Repeat each minute

    // Infinite game loop
    bool isRunning = true, menustart = true, menuload = false, menugame = false; 
    bool generate_random = true, loadmenu_updated = false;
    while (isRunning)
    {
        eventlist cur_event = eventlist::nothing;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) 
            {
                cur_event = eventlist::quit;
                break;
            }
            else if (event.type == SDL_MOUSEMOTION) 
            {
                cur_event = eventlist::motion;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) 
            {
                cur_event = eventlist::press;
                break;
            }
            else 
            {
                cur_event = eventlist::nothing;
                break;
            }
        }

        //------EVENT_HANDLING-------

        if (menugame) // Menu of the game
        {
            generate_random = false;
            G.gameMenu.deal_with_menu(cur_event, isRunning, menuload, menugame, 
                                      G.parser, G.gameMap, G.resourceList, generate_random, loadmenu_updated);
            if (!menugame && generate_random)
            {
                G.construct();
            }
            //---------RENDERING---------
            SDL_RenderClear(renderer);
            G.gameMenu.render(renderer);
        }
        else if (menuload) // Menu for loading saved games
        {
            if (!loadmenu_updated)
            {
                chooseMenu.clear_menu();
                std::string filename;
                // Look at choise menu
                std::string path = "src/worlds";
                // Fill the options of choiceMenu with what is in worlds directory
                for (const auto & world : std::filesystem::directory_iterator(path))
                    chooseMenu.add_option(renderer, world.path());
                loadmenu_updated = true;
            }
            std::string file;
            chooseMenu.deal_with_load(cur_event, isRunning, menustart, file);
            if (!file.empty())
            {
                G.construct(CParser::read_map(file));
                menuload = false;
            }
            //---------RENDERING---------
            SDL_RenderClear(renderer);
            chooseMenu.render(renderer);
        }
        else if (menustart) // Initial game menu
        {
            initMenu.deal_with_init_menu(cur_event, isRunning, menustart, menuload, generate_random);
            if (!menustart && generate_random)
            {
                G.construct();
            }
            else if (!generate_random) loadmenu_updated = false;
            //---------RENDERING---------
            SDL_RenderClear(renderer);
            initMenu.render(renderer);
        }
        else // Actual game
        {
            int Wx = -1, Wy = -1, Mx = -1, My = -1;
            if (cur_event == eventlist::quit)
            {
                isRunning = false;
            }
            else if (cur_event == eventlist::motion)
            {
                SDL_GetMouseState(&Mx, &My);
                G.gameMap.get_cell_coord(Wx, Wy, Mx, My); 
                G.gameMap.set_hover_tile(Wx, Wy);

                G.buttonList.set_hovering(G.buttonList.get_hovering_index(Mx, My));
            }
            else if (cur_event == eventlist::press)
            {
                // 1) Get mouse location
                SDL_GetMouseState(&Mx, &My);
                // 2) See at which part of the screen is the mouse
                if  (Mx <= 128 && My <= 384)    // Button part
                {
                    int index = G.buttonList.get_hovering_index(Mx, My);

                    if (index == G.buttonList.get_selected())
                        G.buttonList.set_selected(-1);
                    else
                        G.buttonList.set_selected(index);

                    if (G.buttonList.get_selected() == 0)
                    {
                        G.buttonList.set_selected(-1);
                        menugame = true;
                    }
                }
                else                            // Other / Map
                {
                    G.gameMap.get_cell_coord(Wx, Wy, Mx, My);
                    switch (G.buttonList.get_selected())
                    {
                        case 2:
                            // Demolish
                            G.gameMap.build(Wx, Wy, -1, G.buildingMap, G.resourceList);
                            break;
                        case 4:
                            // House
                            G.gameMap.build(Wx, Wy, 0, G.buildingMap, G.resourceList);
                            break;
                        case 5:
                            // Mine
                            G.gameMap.build(Wx, Wy, 2, G.buildingMap, G.resourceList);
                            break;
                        case 6:
                            // Fisher
                            G.gameMap.build(Wx, Wy, 3, G.buildingMap, G.resourceList);
                            break;
                        case 7:
                            // Farm
                            G.gameMap.build(Wx, Wy, 4, G.buildingMap, G.resourceList);
                            break;
                        case 8:
                            // Lumber
                            G.gameMap.build(Wx, Wy, 1, G.buildingMap, G.resourceList);
                            break;
                    }
                }
            }
            //---------RENDERING---------
            SDL_RenderClear(renderer);
            G.gameMap.render(renderer);
            G.buttonList.render(renderer);

            G.resourceBar.render_text(renderer);
            G.resourceBar.render_numbers(renderer);
        }
        // Present what has been renderer to the screen
        SDL_RenderPresent(renderer);
    }

    // CLOSE AND FREE EVERYTHING
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

//    TTF_Quit();
    SDL_Quit();

    return 0;
}

// Buildings:
// 0 - house;
// 1 - lumber;
// 2 - mine;
// 3 - fisher;
// 4 - farm;