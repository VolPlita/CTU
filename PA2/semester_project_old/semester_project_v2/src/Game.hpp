#pragma once
#include "Menu.hpp"
#include "Buttons.hpp"
#include "Actions.hpp"

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

    std::vector<std::shared_ptr<CAction>> m_actions;

// Empty map
    CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer *& renderer);
// Load from CWorldData
    CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer *& renderer, CWorldData data);

    void construct();
    void construct(CWorldData data);
    
    void add_flood(Uint32 interval);
    void add_treegrow(Uint32 interval);
};