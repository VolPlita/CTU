#include "Text.hpp"

CText::CText() : m_texture(nullptr), m_font(nullptr) {}
CText::CText(SDL_Renderer *& rend, int x, int y, const std::string & text) : 
            m_texture(nullptr), m_font(nullptr), m_text(text)
{
    // Initializing ttf library
    if (!is_initialized && TTF_Init() == -1)
        std::cout << "Failed to initialize fonts: " << SDL_GetError() << std::endl;
    else
        is_initialized = true;

    // Load font file and set the font size
    m_font = TTF_OpenFont("src/fonts/pixeboy.ttf", 64);
    // Check if it was loaded
    if (m_font == nullptr)
    {
        std::cout << "Failed to open font" << std::endl;
        exit(1);
    }

    // Creating surface from text
    SDL_Surface * text_surface = TTF_RenderText_Solid(m_font, m_text.c_str(), {255, 255, 255, 0});
    // Create texture from surface
    m_texture = SDL_CreateTextureFromSurface(rend, text_surface);
    // Free surface after texture was created
    SDL_FreeSurface(text_surface);
    // Set rectangle on where to display
    m_dst_rect.x = x;
    m_dst_rect.y = y;
    m_dst_rect.w = text_surface->w;
    m_dst_rect.h = text_surface->h;
}
CText::~CText() 
{
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr; 
    TTF_CloseFont(m_font);
    m_font = nullptr;
}
void CText::change_text(SDL_Renderer *& rend, const std::string & text)
{
    if (m_text == text) return;
    m_text = text;
    SDL_Surface * text_surface = TTF_RenderText_Solid(m_font, m_text.c_str(), {255, 255, 255, 0});
    // Create texture from surface
    m_texture = SDL_CreateTextureFromSurface(rend, text_surface);
    // Free surface after texture was created
    SDL_FreeSurface(text_surface);
}
void CText::change_color(SDL_Renderer *& rend, SDL_Color color)
{
    // std::cout << color.r << std::endl;
    // std::cout << color.g << std::endl;
    // std::cout << color.b << std::endl;
    // std::cout << color.a << std::endl;
    SDL_Surface * text_surface = TTF_RenderText_Solid(m_font, m_text.c_str(), color);
    // Create texture from surface
    m_texture = SDL_CreateTextureFromSurface(rend, text_surface);
    // Free surface after texture was created
    SDL_FreeSurface(text_surface);
}

int CText::get_x() const {return m_dst_rect.x;}
int CText::get_y() const {return m_dst_rect.y;}
int CText::get_w() const {return m_dst_rect.w;}
int CText::get_h() const {return m_dst_rect.h;}

std::string CText::get_string()
{
    return m_text;
}
// void CText::show_rect()
// {
//     std::cout << "(" << m_dst_rect.x << "," << m_dst_rect.y << "," << m_dst_rect.w << "," << m_dst_rect.h << ")" << std::endl;
// }

void CText::render(SDL_Renderer *& rend)
{
    // Render the text on some rectangle
    SDL_RenderCopy(rend, m_texture, nullptr, &m_dst_rect);
}

bool CText::is_initialized = false;