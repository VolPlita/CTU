#pragma once
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>

// class CHouse is an experimental class, for me to see how SDL works
class CHouse
{
    double m_x;
    double m_y;
    SDL_Rect m_currentFrame;
    SDL_Texture * m_texture;
public:
    CHouse(double x, double y, SDL_Texture * tx);
    double get_x();
    double get_y();
    SDL_Texture * get_tex();
    SDL_Rect get_currentFrame();
};

class CBuilding {
public:
    int m_cost;
    int m_inputA;
    int m_inputB;
    int m_outputA;
    int m_outputB;
    int m_numWorkers;
    bool m_isBuilt;
    CBuilding();
    ~CBuilding();
    virtual void Build() = 0;
    virtual void Upgrade() = 0;
    virtual void Demolish() = 0;
    virtual void AssignWorker() = 0;
    virtual void UnassignWorker() = 0;
};

class CResidental : public CBuilding
{
    int m_numResidents;
public:
    CResidental();
    ~CResidental();
    void SetNumResidents();
};
class CLumberjack : public CBuilding
{
    int m_numTrees;
public:
    CLumberjack();
    ~CLumberjack();
    void SetNumTrees();
};
class CFarm : public CBuilding
{
    int m_numCrops;
public:
    CFarm();
    ~CFarm();
    void SetNumCrops();
};
class CMine : public CBuilding
{
    int m_numOres;
public:
    CMine();
    ~CMine();
    void SetNumOres();
};