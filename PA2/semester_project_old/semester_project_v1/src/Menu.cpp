#include "Menu.hpp"

CMenu::CMenu() : m_colors{{255, 255, 255, 0}, {255, 255, 0, 0}} {}
// CMenu::~CMenu() {}

void CMenu::add_option(SDL_Renderer *& rend, const std::string& opt)
{
    m_options.push_back(std::make_shared<CText>(rend, SCREEN_WIDTH / 4, 
                                                TILE_WIDTH * (m_options.size() + 1), opt));
    m_selected.push_back(false);
}

void CMenu::clear_menu()
{
    m_options.clear();
}

void CMenu::render(SDL_Renderer *& rend)
{
    for(size_t i = 0, l = m_options.size(); i < l; ++i)
    {
        if (m_selected[i]) 
            m_options[i]->change_color(rend, m_colors[1]);
        else
            m_options[i]->change_color(rend, m_colors[0]);

        m_options[i]->render(rend);
    }
}
void CMenu::deal_with_menu(eventlist event, bool & isRunning, bool & menuload, bool & menugame, 
                           CParser & parser, CTileMap & map, CResources & res, bool & generate_rand, bool & load_updated)
{   
    if (event == eventlist::quit)
    {
        isRunning = false;
    }
    if (event == eventlist::motion)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (size_t i = 0, l = m_selected.size(); i < l; i++)
        {
            m_selected[i] = false;
        }
        if (x > SCREEN_WIDTH / 4 && y > TILE_WIDTH)
        {
            int index = y / TILE_WIDTH - 1;
            if (index >= 0 && (size_t)index < m_options.size())
            {
                if (x < m_options[index]->get_x() + m_options[index]->get_w())
                    m_selected[index] = true;
            }
        }
    }
    if (event == eventlist::press)
    {
        if (m_selected[0])
        {
            // New
            generate_rand = true;
            menugame = false;
        }
        else if (m_selected[1])
        {
            // Load
            menuload = true;
            menugame = false;
        }
        else if (m_selected[2])
        {
            // Save
            parser.save_map(map, res);
            load_updated = false;
        }
        else if (m_selected[3]) 
        {
            // Continue
            menugame = false;
        }
    }
}

void CMenu::deal_with_init_menu(eventlist event, bool & isRunning, bool & menustart, bool & menuload, bool & generate_rand)
{
    if (event == eventlist::quit)
    {
        isRunning = false;
    }
    if (event == eventlist::motion)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (size_t i = 0, l = m_selected.size(); i < l; i++)
        {
            m_selected[i] = false;
        }
        if (x > SCREEN_WIDTH / 4 && y > TILE_WIDTH)
        {
            int index = y / TILE_WIDTH - 1;
            if (index >= 0 && (size_t)index < m_options.size())
            {
                if (x < m_options[index]->get_x() + m_options[index]->get_w())
                    m_selected[index] = true;
            }
        }
    }
    if (event == eventlist::press)
    {
        if (m_selected[0])
        {
            // New
            generate_rand = true;
            menustart = false;
        }
        else if (m_selected[1])
        {
            // Load
            menuload = true;
            generate_rand = false;
            menustart = false;
        }
        else if (m_selected[2])
        {
            //Exit
            menustart = false;
            isRunning = false;
        }
    }
}

void CMenu::deal_with_load(eventlist event, bool & isRunning, bool & menuload, std::string& filename)
{
    if (event == eventlist::quit)
    {
        isRunning = false;
    }
    if (event == eventlist::motion)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (size_t i = 0, l = m_selected.size(); i < l; i++)
        {
            m_selected[i] = false;
        }
        if (x > SCREEN_WIDTH / 4 && y > TILE_WIDTH)
        {
            int index = y / TILE_WIDTH - 1;
            if (index >= 0 && (size_t)index < m_options.size())
            {
                if (x < m_options[index]->get_x() + m_options[index]->get_w())
                    m_selected[index] = true;
            }
        }
    }
    if (event == eventlist::press)
    {
        for (size_t i = 0, l = m_selected.size(); i < l; i++)
        {
            if (m_selected[i])
            {
                menuload = false;
                filename = m_options[i]->get_string();
                return;
            }
        }
    }
    filename = "";
}
