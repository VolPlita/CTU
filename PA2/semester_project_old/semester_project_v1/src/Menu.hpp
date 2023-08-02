#pragma once
#include <memory>
#include <vector>
#include <SDL2/SDL.h>

#include "define.hpp"
#include "Text.hpp"
#include "Parser.hpp"

class CMenu
{
    std::vector<std::shared_ptr<CText>> m_options;
    std::vector<bool> m_selected;
    SDL_Color m_colors[2];
public:
    CMenu();
    // ~CMenu();
    void add_option(SDL_Renderer *& rend, const std::string& opt);
    void clear_menu();
    void render(SDL_Renderer *& rend);
    void deal_with_menu(eventlist event, bool & isRunning, bool & menuload, bool & menugame, 
                        CParser & parser, CTileMap & map, CResources & res, bool & generate_rand, bool & load_updated);
    void deal_with_init_menu(eventlist event, bool & isRunning, bool & menustart, bool & menuload, bool & generate_rand);
    void deal_with_load(eventlist event, bool & isRunning, bool & menuload, std::string& filename);
};
