#pragma once
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>

#include "Resources.hpp"

//Do i need static const int type???

Uint32 callback(Uint32 interval, void * param);
class ABuilding
{
protected:
    int m_price_gold;
    int m_price_stone;
    int m_price_wood;
    CResources & m_resource_base;
    SDL_TimerID m_timer;
    Uint32 m_interval;
public:
    ABuilding(int g, int s, int w, CResources & res_base, Uint32 interval);
    ~ABuilding();
    virtual void update_resources();
    void reset_timer();
};
class CHalupa : public ABuilding // 0
{
    int m_residents_amount;
public: 
    static const int type = 0;
    CHalupa(int g, int s, int w, CResources & res_base, Uint32 interval, int people);
    void update_resources() override;
};
class CLumber : public ABuilding // 1
{
public:
    static const int type = 1;
    CLumber(int g, int s, int w, CResources & res_base, Uint32 interval);
    void update_resources() override;
};
class CMine : public ABuilding // 2
{
    bool m_gold;
public:
    static const int type = 2;
    CMine(int g, int s, int w, CResources & res_base, Uint32 interval, bool gold);
    void update_resources() override;
};
class CFisher : public ABuilding // 3
{
public:
    static const int type = 3;
    CFisher(int g, int s, int w, CResources & res_base, Uint32 interval);
    void update_resources() override;
};
class CFarm : public ABuilding // 4
{
public:
    static const int type = 4;
    CFarm(int g, int s, int w, CResources & res_base, Uint32 interval);
    void update_resources() override;
};
//----------------------------------------------------------------------------------------------------
class CBuildingMap
{
    std::map<int, std::shared_ptr<ABuilding>> m_building_map;
public:
    static int m_max_id;
    //CBuildingMap() {}
    // ~CBuildingMap()
    // {
    //     // Reset pointers
    //     //for (<int, shared_ptr<ABuilding>> : m_building_map)
    // }
    template <class T>
    int add_building(std::shared_ptr<T> building)
    {
        m_building_map.insert({m_max_id, building});
        return m_max_id++;    
    }
    template <class T>
    int add_building_id(int id, std::shared_ptr<T> building)
    {
        m_building_map.insert({id, building});
        if (id > m_max_id) m_max_id = id + 1;
        return m_max_id;    
    }
    void remove_building(int id);
    void clear();
};