#pragma once
#include "Menu.hpp"
#include "Buttons.hpp"
#include "Actions.hpp"


/**
 * @class CGame
 * @brief Represents the game and its components.
 *
 * The CGame class represents the game and includes various components such as the game menu, button list, parser, game map, resource list, building map, and resource bar.
 */
class CGame
{
public:
    CMenu gameMenu;                   ///< The game menu object for managing the game menu.
    CButtonList buttonList;           ///< The button list object for managing game buttons.
    CParser parser;                   ///< The parser object for parsing game data.

    CTileMap gameMap;                 ///< The game map object representing the game world.
    CResources resourceList;          ///< The resource list object for managing game resources.
    CBuildingMap buildingMap;         ///< The building map object for managing game buildings.
    CResourceText resourceBar;        ///< The resource bar object for displaying the available resources.

    std::vector<std::shared_ptr<CAction>> m_actions;  ///< Vector to store game actions as shared pointers to CAction objects.

    /**
     * @brief Default constructor for CGame class with empty map.
     * @param tiletexture The filename of the texture for tiles.
     * @param buttontexture The filename of the texture for buttons.
     * @param renderer The SDL_Renderer used for rendering.
     *
     * Creates an empty game map and initializes the game menu, button list, parser, resource list, building map, and resource bar.
     */
    CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer * renderer);

    /**
     * @brief Constructor for CGame class that loads data from CWorldData.
     * @param tiletexture The filename of the texture for tiles.
     * @param buttontexture The filename of the texture for buttons.
     * @param renderer The SDL_Renderer used for rendering.
     * @param data The CWorldData object containing game data.
     *
     * Creates a game map and initializes the game menu, button list, parser, resource list, building map, and resource bar using the provided data.
     */
    CGame(const std::string & tiletexture, const std::string & buttontexture, SDL_Renderer *& renderer, CWorldData data);

    /**
     * @brief Constructs the game components.
     *
     * Constructs the game components by initializing the game menu, button list, parser, resource list, building map, and resource bar.
     */
    void construct();

    /**
     * @brief Constructs the game components using the provided data.
     * @param data The CWorldData object containing game data.
     *
     * Constructs the game components using the provided data by initializing the game menu, button list, parser, resource list, building map, and resource bar.
     */
    void construct(CWorldData data);
    
    /**
     * @brief Adds a flood action to the game.
     * @param interval The interval at which the flood action should occur.
     *
     * Adds a flood action to the game with the specified interval.
     */
    void add_flood(Uint32 interval);
    
    /**
     * @brief Adds a tree growth action to the game.
     * @param interval The interval at which the tree growth action should occur.
     *
     * Adds a tree growth action to the game with the specified interval.
     */
    void add_treegrow(Uint32 interval);
};