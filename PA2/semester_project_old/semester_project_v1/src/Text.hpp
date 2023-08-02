#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class CText
{
    static bool is_initialized;
    //SDL_Renderer *& renderer;
    SDL_Texture * m_texture;
    TTF_Font * m_font;
    SDL_Rect m_dst_rect;
    std::string m_text;
public:
    CText();
    CText(SDL_Renderer *& rend, int x, int y, const std::string & text);
    ~CText();
    void change_text(SDL_Renderer *& rend, const std::string & text);
    void change_color(SDL_Renderer *& rend, SDL_Color color);

    int get_x() const;
    int get_y() const;
    int get_w() const;
    int get_h() const;

    std::string get_string();

    void render(SDL_Renderer *& rend);
};