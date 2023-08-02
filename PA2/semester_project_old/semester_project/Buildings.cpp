#include <iostream>
#include "Buildings.hpp"

// class CHouse is an experimental class, for me to see how SDL works
CHouse::CHouse(double x, double y, SDL_Texture * tx) : m_x(x), m_y(y), m_texture(tx)
{
    m_currentFrame.x = 0;
    m_currentFrame.y = 0;
    m_currentFrame.w = 100;
    m_currentFrame.h = 100;
}
double CHouse::get_x()
{
    return m_x;
}
double CHouse::get_y()
{
    return m_y;
}
SDL_Texture * CHouse::get_tex()
{
    return m_texture;
}
SDL_Rect CHouse::get_currentFrame()
{
    return m_currentFrame;
}
