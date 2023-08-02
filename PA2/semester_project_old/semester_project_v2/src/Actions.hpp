#pragma once
#include <SDL2/SDL.h>
#include "Tiles.hpp"

Uint32 callback_action(Uint32 interval, void * param);

class CAction
{
protected:
    CTileMap & m_map_ref;
    CBuildingMap & m_build_ref;
    SDL_TimerID m_timer;
    Uint32 m_interval;
public:
    CAction(CTileMap & map, CBuildingMap & build, Uint32 interval);
    ~CAction();
    void reset_timer();
    virtual void empose_effect() const = 0;
};

class CFlood : public CAction
{
public:
    CFlood(CTileMap & map, CBuildingMap & build, Uint32 interval);
    void empose_effect() const override;
};

class CTreeGrowth : public CAction
{
public:
    CTreeGrowth(CTileMap & map, CBuildingMap & build, Uint32 interval);
    void empose_effect() const override;
};