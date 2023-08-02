#include "Buildings.hpp"

Uint32 callback(Uint32 interval, void * param)
{
    ((ABuilding*)param)->update_resources();
    ((ABuilding*)param)->reset_timer();
    return 0;
}
//----------------------------------------------------------------------------------------------------
ABuilding::ABuilding(int g, int s, int w, CResources & res_base, Uint32 interval) : 
m_price_gold(g), m_price_stone(s), m_price_wood(w), m_resource_base(res_base), m_interval(interval)
{
    m_resource_base.remove_resource("Gold", m_price_gold);
    m_resource_base.remove_resource("Stone", m_price_stone);
    m_resource_base.remove_resource("Wood", m_price_wood);
    // auto callmebabe = new std::function<void()>([&]() {
    //     this->update_resources();
    // });
    m_timer = SDL_AddTimer(m_interval, callback, this);
}
ABuilding::~ABuilding()
{
    SDL_RemoveTimer(m_timer);
}
void ABuilding::update_resources() 
{
    std::cout << "I am here" << std::endl;
}
void ABuilding::reset_timer()
{
    SDL_RemoveTimer(m_timer);
    m_timer = SDL_AddTimer(m_interval, callback, this);
}
//----------------------------------------------------------------------------------------------------
CHalupa::CHalupa(int g, int s, int w, CResources & res_base, Uint32 interval, int people) : ABuilding(g, s, w, res_base, interval), m_residents_amount(people) 
{
    m_resource_base.add_resource("Population", 1);
}
void CHalupa::update_resources() {m_resource_base.add_resource("Gold", 1);}
//----------------------------------------------------------------------------------------------------
CLumber::CLumber(int g, int s, int w, CResources & res_base, Uint32 interval) : ABuilding(g, s, w, res_base, interval) {}
void CLumber::update_resources() 
{
    m_resource_base.add_resource("Wood", 5);
    // std::cout << "Lumber!!!" << std::endl;
}
//----------------------------------------------------------------------------------------------------
CMine::CMine(int g, int s, int w, CResources & res_base, Uint32 interval, bool gold) : ABuilding(g, s, w, res_base, interval), m_gold(gold) {}
void CMine::update_resources() 
{
    if (m_gold)
        m_resource_base.add_resource("Gold", 3);
    else
        m_resource_base.add_resource("Stone", 5);
    // std::cout << "Mine!!!" << std::endl;
}
//----------------------------------------------------------------------------------------------------
CFisher::CFisher(int g, int s, int w, CResources & res_base, Uint32 interval) : ABuilding(g, s, w, res_base, interval) {}
void CFisher::update_resources() 
{
    m_resource_base.add_resource("Fish", 5);
    // std::cout << "Fisher!!!" << std::endl;
}
//----------------------------------------------------------------------------------------------------
CFarm::CFarm(int g, int s, int w, CResources & res_base, Uint32 interval) : ABuilding(g, s, w, res_base, interval) {}
void CFarm::update_resources() 
{
    m_resource_base.add_resource("Bread", 5);
    // std::cout << "Farm!!!" << std::endl;
}
//----------------------------------------------------------------------------------------------------
int CBuildingMap::m_max_id = 0;
void CBuildingMap::remove_building(int id)
{
    m_building_map[id].reset();
    m_building_map.erase(id);
}
void CBuildingMap::clear()
{
    m_building_map.clear();
}
