#pragma once
#include <vector>
#include <SDL2/SDL.h>

class CButton
{
    SDL_Rect m_sprite;
    SDL_Rect m_dst_rect;
public:
    CButton(int sx, int sy, int dx, int dy);
    void render(SDL_Renderer *& rend, SDL_Texture * texture_set) const;
    SDL_Rect const & get_dst() const;
};

class CButtonList
{
    SDL_Texture * m_texture_set;
    SDL_Rect m_hover_sprite;
    SDL_Rect m_select_sprite;
    std::vector<CButton> m_button_list;
    int m_selected;
    int m_hovering;
public:
    CButtonList(SDL_Renderer *& rend, const char * texture_src);
    ~CButtonList();
    void add_button(int sx, int sy, int dx, int dy);
    void set_selected(int x);
    void set_hovering(int x);
    void render(SDL_Renderer *& rend) const;
    int get_selected() const;
    int get_hovering_index(int x, int y) const;
    void init_list();
};