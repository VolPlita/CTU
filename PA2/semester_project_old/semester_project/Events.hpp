#pragma once
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <string>

class IEvent {
    std::string m_name;
    std::string m_description;
    int m_duration;
public:
    IEvent();
    ~IEvent();
    virtual void start() = 0;
    virtual void end() = 0;
    virtual void update() = 0;
    virtual bool isActive() = 0;
    virtual void applyEffect() = 0;
    virtual void revertEffect() = 0;
};
class CHuricane : public IEvent 
{
    int m_damageLevel;
    int m_duration;
public:
    CHuricane();
    ~CHuricane();
    void applyEffect();
    void end();
};
class CBlessing : public IEvent 
{
    int m_blessingLevel;
    int m_duration;
public:
    CBlessing();
    ~CBlessing();
    void applyEffect();
    void end();
};
class CFamine : public IEvent 
{
    int m_intensity;
    int m_duration;
public:
    CFamine();
    ~CFamine();
    void applyEffect();
    void end();
};