#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <filesystem>

#include "define.hpp"
#include "Text.hpp"
#include "Resources.hpp"
#include "Buildings.hpp"
#include "Tiles.hpp"
#include "Buttons.hpp"
#include "Menu.hpp"
#include "Parser.hpp"
#include "Game.hpp"
#include "Actions.hpp"

void init_SDL(SDL_Window * win, SDL_Renderer *& ren)
{
    win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(ren, 0x66, 0x66, 0xBB, 0xFF);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char * argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cout << "Fail on initializing SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    init_SDL(window, renderer);

    CMenu chooseMenu;
    // Without options for the start

    CMenu initMenu;
    initMenu.add_option(renderer, "New game");
    initMenu.add_option(renderer, "Load game");
    initMenu.add_option(renderer, "Exit");

    CGame G("src/images/textures.bmp", "src/images/buttons.bmp", renderer);  
    G.add_flood(1000 * 60); // Repeat each minute
    G.add_treegrow(1000 * 60); // Repeat each minute

    //Infinite game loop
    // To render before I need new renderer!!!!!!!!!!!!!!
    bool isRunning = true, menustart = true, menuload = false, menugame = false; 
    bool generate_random = true, loadmenu_updated = false;
    //showMenu = false;
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
                for (const auto & world : std::filesystem::directory_iterator(path))
                    chooseMenu.add_option(renderer, world.path());
                loadmenu_updated = true;
            }
            std::string file;
            // std::cout << "R=" << isRunning << " S=" << menustart << " L=" << menuload << " G=" << menugame << std::endl;
            chooseMenu.deal_with_load(cur_event, isRunning, menustart, file);
            // std::cout << "R=" << isRunning << " S=" << menustart << " L=" << menuload << " G=" << menugame << std::endl;
            // std::cout << file.empty() << std::endl;
            if (!file.empty())
            {
                // G.construct();
                G.construct(G.parser.read_map(file));
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
                // int Mx, My;
                SDL_GetMouseState(&Mx, &My);
                G.gameMap.get_cell_coord(Wx, Wy, Mx, My); 
                G.gameMap.set_hover_tile(Wx, Wy);

                G.buttonList.set_hovering(G.buttonList.get_hovering_index(Mx, My));
            }
            else if (cur_event == eventlist::press)
            {
                // std::cout << "Here";
                // 1) Get mouse location
                // int Mx, My;
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

    return 0;
}

// Buildings:
// 0 - house;
// 1 - lumber;
// 2 - mine;
// 3 - fisher;
// 4 - farm;