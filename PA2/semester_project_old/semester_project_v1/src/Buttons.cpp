#include "Buttons.hpp"

CButton::CButton(int sx, int sy, int dx, int dy) 
{
    m_sprite.h = m_sprite.w = m_dst_rect.h = m_dst_rect.w = 64;
    m_sprite.x = sx;
    m_sprite.y = sy;
    m_dst_rect.x = dx;
    m_dst_rect.y = dy;
}
void CButton::render(SDL_Renderer *& rend, SDL_Texture * texture_set) const
{
    SDL_RenderCopy(rend, texture_set, &m_sprite, &m_dst_rect);
}
SDL_Rect const & CButton::get_dst() const {return m_dst_rect;}
//-------------------------------------------------------------------------------------------------------------
CButtonList::CButtonList(SDL_Renderer *& rend, const char * texture_src) : m_selected(-1) , m_hovering(-1)
{
    SDL_Surface * tilemap_surface = SDL_LoadBMP(texture_src);
    m_texture_set = SDL_CreateTextureFromSurface(rend, tilemap_surface);
    SDL_FreeSurface(tilemap_surface);
     
    m_hover_sprite.h = m_hover_sprite.w = m_select_sprite.h = m_select_sprite.w = m_select_sprite.y = 64;
    m_hover_sprite.x = m_hover_sprite.y = m_select_sprite.x = 0;
}
CButtonList::~CButtonList()
{
    SDL_DestroyTexture(m_texture_set);
    m_texture_set = nullptr;
}
void CButtonList::add_button(int sx, int sy, int dx, int dy) 
{
    m_button_list.emplace_back(sx, sy, dx, dy);
}
void CButtonList::set_selected(int x) {m_selected = x;}
void CButtonList::set_hovering(int x) {m_hovering = x;}
void CButtonList::render(SDL_Renderer *& rend) const 
{
    for (int i = 0; i < 4; ++i)
    {
        m_button_list[i].render(rend, m_texture_set);
    }
    if (m_selected == 1 || m_selected > 3)
    {
        for (int i = 4, l = static_cast<int>(m_button_list.size()); i < l; ++i)
        {
            m_button_list[i].render(rend, m_texture_set);
        }
    }
    if (m_hovering >= 0 && m_hovering < static_cast<int>(m_button_list.size()))
        SDL_RenderCopy(rend, m_texture_set, &m_hover_sprite, &m_button_list[m_hovering].get_dst());
    if (m_selected >= 0 && m_selected < static_cast<int>(m_button_list.size()))
        SDL_RenderCopy(rend, m_texture_set, &m_select_sprite, &m_button_list[m_selected].get_dst());
}
int CButtonList::get_selected() const {return m_selected;}
void CButtonList::init_list()
{
    this->add_button(64, 0, 0, 0);          // Settings - 0
    this->add_button(128, 0, 0, 64);        // Build - 1
    this->add_button(192, 0, 0, 128);       // Demolish - 2
    this->add_button(64, 64, 0, 192);       // Move - 3
    this->add_button(128, 64, 64, 64);      // House - 4
    this->add_button(192, 64, 64, 128);     // Mine - 5
    this->add_button(0, 128, 64, 192);      // Fisher - 6
    this->add_button(64, 128, 64, 256);     // Farm - 7
    this->add_button(128, 128, 64, 320);    // Lumber - 8
}

int CButtonList::get_hovering_index(int x, int y) const
{
    if (m_selected == 1 || m_selected > 3)
    {
        if (x < 64 && y < 256) return y / 64;
        else if (x < 128 && y < 384 && x > 64 && y > 64) return y / 64 + 3;
        else return -1;
    }
    else 
    {
        if (x < 64 && y < 256) return y / 64;
        else return -1;
    }
}
