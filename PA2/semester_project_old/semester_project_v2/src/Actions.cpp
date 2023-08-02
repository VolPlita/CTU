#include "Actions.hpp"

Uint32 callback_action(Uint32 interval, void * param)
{
    ((CAction*)param)->empose_effect();
    ((CAction*)param)->reset_timer();
    return 0;
}

CAction::CAction(CTileMap & map, CBuildingMap & build, Uint32 interval) : m_map_ref(map), m_build_ref(build), m_interval(interval)
{
    m_timer = SDL_AddTimer(m_interval, callback_action, this);
}
CAction::~CAction()
{
    SDL_RemoveTimer(m_timer);
}
void CAction::reset_timer()
{
    SDL_RemoveTimer(m_timer);
    m_timer = SDL_AddTimer(m_interval, callback_action, this);
}

CFlood::CFlood(CTileMap & map, CBuildingMap & build, Uint32 interval) : CAction(map, build, interval) {}
void CFlood::empose_effect() const
{
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            if (m_map_ref.access_map()->at(i)[j].get_type() == 3)
            {
                if (rand() % 10 == 0)
                {
                    int id = m_map_ref.access_map()->at(i)[j].get_building_id();
                    m_build_ref.remove_building(id);
                    m_map_ref.access_map()->at(i)[j].set_type(2);
                    m_map_ref.access_map()->at(i)[j].set_building(-1);
                    m_map_ref.access_map()->at(i)[j].set_building_id(-1);
                    m_map_ref.access_map()->at(i)[j].set_resource(-1);
                }
            }
        }
    }
}

CTreeGrowth::CTreeGrowth(CTileMap & map, CBuildingMap & build, Uint32 interval) : CAction(map, build, interval) {}
void CTreeGrowth::empose_effect() const
{
    std::cout << "Pock!!" << std::endl;
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            if ((m_map_ref.access_map()->at(i)[j].get_type() == 0 || m_map_ref.access_map()->at(i)[j].get_type() == 1) &&
                (m_map_ref.access_map()->at(i)[j].get_resource() == -1 || m_map_ref.access_map()->at(i)[j].get_building() == -1))
            {
                std::cout << "Rock!!" << std::endl;
                if (rand() % 10 == 0)
                {
                    m_map_ref.access_map()->at(i)[j].set_resource(rand() % 2 + 2);
                }
            }
        }
    }
}