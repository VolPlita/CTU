#ifndef TEXT_HPP
#define TEXT_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

class CText
{
    static bool is_initialized;
    SDL_Renderer * renderer;
    SDL_Texture * m_texture;
    TTF_Font * m_font;
    SDL_Rect m_dst_rect;
    std::string m_text;
public:
    CText(SDL_Renderer * rend, int x, int y, int w, int h, const std::string & text) : renderer(rend), m_texture(nullptr), m_font(nullptr), m_text(text)
    {
        // Initializing ttf library
        if (!is_initialized && TTF_Init() == -1)
            std::cout << "Failed to initialize fonts: " << SDL_GetError() << std::endl;
        else
            is_initialized = true;
        // Load font file and set the font size
        TTF_Font * m_font = TTF_OpenFont("fonts/pixeboy.ttf", 16);
        // Check if it was loaded
        if (m_font == nullptr)
        {
            std::cout << "Failed to open font" << std::endl;
            exit(1);
        }

        // Creating surface from text
        SDL_Surface * text_surface = TTF_RenderText_Solid(m_font, m_text.c_str(), {255, 255, 255});
        // Create texture from surface
        SDL_Texture * m_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        // Free surface after texture was created
        SDL_FreeSurface(text_surface);

        // Set rectangle on where to display
        m_dst_rect.x = x;
        m_dst_rect.y = y;
        m_dst_rect.w = w;
        m_dst_rect.h = h;
    }
    ~CText() 
    {
        renderer = nullptr;
        SDL_DestroyTexture(m_texture); 
        TTF_CloseFont(m_font);
    }
    void change_text(const std::string & text)
    {
        m_text = text;
    }
    void render()
    {
        // Render the text on some rectangle
        SDL_RenderCopy(renderer, m_texture, nullptr, &m_dst_rect);
    }
};
bool CText::is_initialized = false;

#endif