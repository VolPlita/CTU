#include <iostream>
#include "GameControll.hpp"

CRenderWindow::CRenderWindow(const char * title, int w, int h) : m_window(nullptr), m_renderer(nullptr)
{
    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
        std::cout << "Window failed to initialize" << std::endl;
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

CRenderWindow::~CRenderWindow()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    m_window = nullptr;
    m_renderer = nullptr;
}

SDL_Texture * CRenderWindow::loadTexture(const char * file_path)
{
    SDL_Texture * texture = nullptr;
    texture = IMG_LoadTexture(m_renderer, file_path);
    if (texture == nullptr)
        std::cout << "Failed to load a texture. Error: " << SDL_GetError() << std::endl;
    return texture;
}

void CRenderWindow::clearScreen()
{
    SDL_RenderClear(m_renderer);
}
void CRenderWindow::render(CHouse & h)
{
    SDL_Rect src;
    src.x = h.get_currentFrame().x;
    src.y = h.get_currentFrame().y;
    src.w = h.get_currentFrame().w;
    src.h = h.get_currentFrame().h;

    SDL_Rect dst;
    dst.x = h.get_x() * 4;
    dst.y = h.get_y() * 4;
    dst.w = h.get_currentFrame().w * 4;
    dst.h = h.get_currentFrame().h * 4;

    SDL_RenderCopy(m_renderer, h.get_tex(), &src, &dst);
}
void CRenderWindow::display()
{
    SDL_RenderPresent(m_renderer);
}