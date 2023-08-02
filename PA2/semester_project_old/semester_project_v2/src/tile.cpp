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

class CGame
{
public:
    CMenu gameMenu;
    CButtonList buttonList;
    CParser parser;

    CTileMap gameMap;
    CResources resourceList;
    CBuildingMap buildingMap;
    CResourceText resourceBar;

// Empty map
    CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer *& renderer) 
    : buttonList(renderer, buttontexture.c_str()),
      gameMap(renderer, tiletexture.c_str(), MAP_SIZE, MAP_SIZE),
      resourceBar(&resourceList, renderer)
    {
        //gameMap.populate();
        buttonList.init_list(); // Standart button set

        gameMenu.add_option(renderer, "New Game");
        gameMenu.add_option(renderer, "Load Game");
        gameMenu.add_option(renderer, "Save Game");
        gameMenu.add_option(renderer, "Continue");
    }
// Load from CWorldData
    CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer *& renderer, CWorldData data) 
    : buttonList(renderer, buttontexture.c_str()),
      gameMap(renderer, tiletexture.c_str(), MAP_SIZE, MAP_SIZE),
      resourceBar(&resourceList, renderer)
    {
        //Map
        gameMap.populate(data);
        // Resources
        resourceList.set_resource("Gold", data.m_res_bar[0]);
        resourceList.set_resource("Stone", data.m_res_bar[1]);
        resourceList.set_resource("Wood", data.m_res_bar[2]);
        resourceList.set_resource("Bread", data.m_res_bar[3]);
        resourceList.set_resource("Fish", data.m_res_bar[4]);
        resourceList.set_resource("Population", data.m_res_bar[5]);
        //Buildings
        buildingMap.m_max_id = 0;
        for (size_t i = 0, l = MAP_SIZE * MAP_SIZE; i < l; i++)
        {
            if (data.m_buildings[i] == 0)
            {
                //Halupa
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CHalupa>(1, 1, 1, resourceList, 10 * 1000, 1));
            }
            else if (data.m_buildings[i] == 1)
            {
                //Lumber
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CLumber>(1, 1, 1, resourceList, 10 * 1000));
            }
            else if (data.m_buildings[i] == 2)
            {
                //Mine
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CMine>(1, 1, 1, resourceList, 10 * 1000, false));
            }
            else if (data.m_buildings[i] == 3)
            {
                //Fisher
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CFisher>(1, 1, 1, resourceList, 10 * 1000));
            }
            else if (data.m_buildings[i] == 4)
            {
                //Farm
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CFarm>(1, 1, 1, resourceList, 10 * 1000));
            }
        }
        
        buttonList.init_list(); // Standart button set

        gameMenu.add_option(renderer, "New Game");
        gameMenu.add_option(renderer, "Load Game");
        gameMenu.add_option(renderer, "Save Game");
        gameMenu.add_option(renderer, "Continue");
    }
    void construct()
    {
        gameMap.populate();
    }
    void construct(CWorldData data)
    {
        //Map
        gameMap.populate(data);
        // Resources
        resourceList.set_resource("Gold", data.m_res_bar[0]);
        resourceList.set_resource("Stone", data.m_res_bar[1]);
        resourceList.set_resource("Wood", data.m_res_bar[2]);
        resourceList.set_resource("Bread", data.m_res_bar[3]);
        resourceList.set_resource("Fish", data.m_res_bar[4]);
        resourceList.set_resource("Population", data.m_res_bar[5]);
        //Buildings
        buildingMap.m_max_id = 0;
        for (size_t i = 0, l = MAP_SIZE * MAP_SIZE; i < l; i++)
        {
            if (data.m_buildings[i] == 0)
            {
                //Halupa
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CHalupa>(1, 1, 1, resourceList, 10 * 1000, 1));
            }
            else if (data.m_buildings[i] == 1)
            {
                //Lumber
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CLumber>(1, 1, 1, resourceList, 10 * 1000));
            }
            else if (data.m_buildings[i] == 2)
            {
                //Mine
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CMine>(1, 1, 1, resourceList, 10 * 1000, false));
            }
            else if (data.m_buildings[i] == 3)
            {
                //Fisher
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CFisher>(1, 1, 1, resourceList, 10 * 1000));
            }
            else if (data.m_buildings[i] == 4)
            {
                //Farm
                buildingMap.add_building_id(data.m_building_ids[i], std::make_shared<CFarm>(1, 1, 1, resourceList, 10 * 1000));
            }
        }
    }
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
void init_SDL(SDL_Window *& win, SDL_Renderer *& ren)
{
    win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char * argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cout << "Fail on initializing SDL: " << SDL_GetError() << std::endl;
    }

    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    init_SDL(window, renderer);

    CMenu initMenu;
    initMenu.add_option(renderer, "New game");
    initMenu.add_option(renderer, "Load game");
    initMenu.add_option(renderer, "Exit");
    CGame G("images/textures.bmp", "images/buttons.bmp", renderer);
    
    bool start = true, isRunning = false, showMenu = false, gen_rand = false;
    while (start)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            initMenu.deal_with_init_menu(event, start, isRunning, gen_rand);
        }
        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);
        initMenu.render(renderer);    
        SDL_RenderPresent(renderer);
    }

    if(!gen_rand)
    {
        std::string filename;
        // Look at choise menu
        CMenu chooseMenu;
        std::string path = "worlds";
        for (const auto & world : std::filesystem::directory_iterator(path))
            chooseMenu.add_option(renderer, world.path());

        bool loadrun = true;
        while(loadrun)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                chooseMenu.deal_with_load(event, loadrun, filename);
            }
            if(!filename.empty()) break;
            SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
            SDL_RenderClear(renderer);
            chooseMenu.render(renderer);
            SDL_RenderPresent(renderer);
        }
        
        G.construct(G.parser.read_map(filename));
    }
    else
    {
        G.construct();
    }

    //Infinite game loop
    // To render before I need new renderer!!!!!!!!!!!!!!
    isRunning = true; 
    //showMenu = false;
    while (isRunning)
    {
        int Wx, Wy = -1; // Currently hovering cell in world coordinates
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }

            if (!showMenu)              // Game Part
            {
                if (event.type == SDL_MOUSEMOTION)
                {
                    int Mx, My;
                    SDL_GetMouseState(&Mx, &My);
                    G.gameMap.get_cell_coord(Wx, Wy, Mx, My); 
                    G.gameMap.set_hover_tile(Wx, Wy);

                    G.buttonList.set_hovering(G.buttonList.get_hovering_index(Mx, My));
                }
                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    // 1) Get mouse location
                    int Mx, My;
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
                            showMenu = true;
                        }
                    }
                    else                            // Other / Map
                    {
                        G.gameMap.get_cell_coord(Wx, Wy, Mx, My);
                        // std::cout << "Wx = " << Wx << " Wy = " << Wy << std::endl;
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
            }
            else                   // Menu part
            {
                G.gameMenu.deal_with_menu(event, showMenu, G.parser, G.gameMap, G.resourceList);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);
        if (showMenu) 
        {
            G.gameMenu.render(renderer);
        }
        else
        {
            // render after each cycle
            G.gameMap.render(renderer);
            G.buttonList.render(renderer);

            G.resourceBar.render_text(renderer);
            G.resourceBar.render_numbers(renderer);
        }
        // Display what has been rendered to a screen
        SDL_RenderPresent(renderer);
    }
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