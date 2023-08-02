#pragma once
#include <memory>
#include <vector>
#include <SDL2/SDL.h>

#include "define.hpp"
#include "Text.hpp"
#include "Parser.hpp"


/**
 * @class CMenu
 * @brief Represents a menu with selectable options.
 *
 * The CMenu class provides functionality for creating and managing a menu with selectable options. It utilizes the SDL library for rendering and user input.
 */
class CMenu
{
    std::vector<std::shared_ptr<CText>> m_options;      ///< Vector to store menu options as shared pointers to CText objects
    std::vector<bool> m_selected;                       ///< Vector to store the selection status of each menu option
    SDL_Color m_colors[2];                              ///< Array to store colors for selected and unselected options
public:

    /**
     * @brief Default constructor for CMenu class.
     *
     * Initializes the menu colors to default values.
     */
    CMenu();

    /**
     * @brief Adds a new option to the menu.
     * @param rend The SDL_Renderer used for rendering.
     * @param opt The text of the option to be added.
     *
     * Creates a new CText object representing the option and adds it to the menu.
     */
    void add_option(SDL_Renderer * rend, const std::string& opt);

    /**
     * @brief Clears all options from the menu.
     *
     * Removes all options from the menu, clearing the menu completely.
     */
    void clear_menu();

    /**
     * @brief Renders the menu on the provided renderer.
     * @param rend The SDL_Renderer used for rendering.
     *
     * Renders the menu options on the specified renderer.
     */
    void render(SDL_Renderer * rend);

    /**
     * @brief Handles events related to the menu in the main menu state.
     * @param event The type of event to handle.
     * @param isRunning A reference to a boolean indicating the running state of the program.
     * @param menuload A reference to a boolean indicating the loading menu state.
     * @param menugame A reference to a boolean indicating the game menu state.
     * @param parser A reference to the CParser object for parsing game data.
     * @param map A reference to the CTileMap object representing the game map.
     * @param res A reference to the CResources object containing game resources.
     * @param generate_rand A reference to a boolean indicating whether to generate a random game.
     * @param load_updated A reference to a boolean indicating whether the game load is updated.
     *
     * Handles different types of events related to the menu when in the main menu state.
     */
    void deal_with_menu(eventlist event, bool & isRunning, bool & menuload, bool & menugame, 
                        CParser & parser, CTileMap & map, CResources & res, bool & generate_rand, bool & load_updated);

                        /**
     * @brief Handles events related to the menu in the initial menu state.
     * @param event The type of event to handle.
     * @param isRunning A reference to a boolean indicating the running state of the program.
     * @param menustart A reference to a boolean indicating the starting menu state.
     * @param menuload A reference to a boolean indicating the loading menu state.
     * @param generate_rand A reference to a boolean indicating whether to generate a random game.
     *
     * Handles different types of events related to the menu when in the initial menu state.
     */
    void deal_with_init_menu(eventlist event, bool & isRunning, bool & menustart, bool & menuload, bool & generate_rand);

    /**
     * @brief Handles events related to loading a game from the menu.
     * @param event The type of event to handle.
     * @param isRunning A reference to a boolean indicating the running state of the program.
     * @param menuload A reference to a boolean indicating the loading menu state.
     * @param filename A reference to a string storing the filename of the loaded game.
     *
     * Handles different types of events related to loading a game from the menu.
     */
    void deal_with_load(eventlist event, bool & isRunning, bool & menuload, std::string& filename);
};
