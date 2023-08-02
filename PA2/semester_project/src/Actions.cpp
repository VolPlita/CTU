#include "Actions.hpp"

Uint32 callback_action(Uint32 interval, void * param)
{
    ((CAction*)param)->empose_effect();
    ((CAction*)param)->reset_timer();
    return 0;
}

CAction::CAction(CTileMap & map, CBuildingMap & build, Uint32 interval) : m_map_ref(map), m_build_ref(build), m_interval(interval)
{
    // Parameter "param" passed is this, because action to call is defined by the class that calls it
    m_timer = SDL_AddTimer(m_interval, callback_action, this);
}
CAction::~CAction()
{
    // Free resources used by timer
    SDL_RemoveTimer(m_timer);
}
void CAction::reset_timer()
{
    // Free resources used by timer
    SDL_RemoveTimer(m_timer);
    // Recreate the timer again
    m_timer = SDL_AddTimer(m_interval, callback_action, this);
}

CFlood::CFlood(CTileMap & map, CBuildingMap & build, Uint32 interval) : CAction(map, build, interval) {}
void CFlood::empose_effect() const
{
    // Loop over every tile in map
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            // If the tile is sand
            if (m_map_ref.access_map()->at(i)[j].get_type() == 3)
            {
                // Make 10% chance that it will be flooded
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
    // Loop over every tile in map
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            // If the tile if grass or flowers and does not containd building or resource
            if ((m_map_ref.access_map()->at(i)[j].get_type() == 0 || m_map_ref.access_map()->at(i)[j].get_type() == 1) &&
                (m_map_ref.access_map()->at(i)[j].get_resource() == -1 || m_map_ref.access_map()->at(i)[j].get_building() == -1))
            {
                // Make 10% chance of growing a tree
                if (rand() % 10 == 0)
                {
                    m_map_ref.access_map()->at(i)[j].set_resource(rand() % 2 + 2);
                }
            }
        }
    }
}