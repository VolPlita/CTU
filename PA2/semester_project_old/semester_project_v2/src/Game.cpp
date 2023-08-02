#include "Game.hpp"

// Empty map
CGame::CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer *& renderer) 
: buttonList(renderer, buttontexture.c_str()),
  gameMap(renderer, tiletexture.c_str(), MAP_SIZE, MAP_SIZE),
  resourceBar(&resourceList, renderer)
{
    buttonList.init_list(); // Standart button set
    gameMenu.add_option(renderer, "New Game");
    gameMenu.add_option(renderer, "Load Game");
    gameMenu.add_option(renderer, "Save Game");
    gameMenu.add_option(renderer, "Continue");
}
// Load from CWorldData
CGame::CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer *& renderer, CWorldData data) 
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
void CGame::construct()
{
    gameMap.populate();
}
void CGame::construct(CWorldData data)
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

void CGame::add_flood(Uint32 interval)
{
    m_actions.push_back(std::make_shared<CFlood>(gameMap, buildingMap, interval));
}
void CGame::add_treegrow(Uint32 interval)
{
    m_actions.push_back(std::make_shared<CTreeGrowth>(gameMap, buildingMap, interval));
}