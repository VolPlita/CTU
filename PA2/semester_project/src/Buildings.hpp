#pragma once
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>

#include "Resources.hpp"

/**
 * @brief Callback function for the building timer. Called each time when building needs to produce resources
 * @param interval The interval in milliseconds.
 * @param param Pointer to the ABuilding object.
 * @return The callback return value.
 */
Uint32 callback(Uint32 interval, void * param);

/**
 *  Base class for buildings. Building can be of five types. Residental house (Halupa), Lumber, Mine, Ambar or Fisher. 
 * Class saves information about buildings price, keeps reference of resource base that it will update and stores timer
 * with time interval for it's timer 
 */
class ABuilding
{
protected:
    int m_price_gold;               ///< Gold price of the building.
    int m_price_stone;              ///< Stone price of the building.
    int m_price_wood;               ///< Wood price of the building.
    CResources & m_resource_base;   ///< Reference to the resource base.
    SDL_TimerID m_timer;            ///< Timer ID.
    Uint32 m_interval;              ///< Time interval for the building.
public:

    /**
     * @brief Constructor for ABuilding class.
     * @param g Gold price of the building.
     * @param s Stone price of the building.
     * @param w Wood price of the building.
     * @param res_base Reference to the resource base.
     * @param interval Time interval for the building.
     */
    ABuilding(int g, int s, int w, CResources & res_base, Uint32 interval);

    /**
     * @brief Destructor for ABuilding class.
     */
    virtual ~ABuilding();

    /**
     * @brief Update the resources of the building.
     */
    virtual void update_resources() = 0;

    /**
     * @brief Reset the building timer.
     */
    void reset_timer();
};

/**
 * @brief Class for the Halupa building.
 */
class CHalupa : public ABuilding // 0
{
    int m_residents_amount;
public: 
    static const int type = 0;

    /**
     * @brief Constructor for CHalupa class.
     * @param g Gold price of the building.
     * @param s Stone price of the building.
     * @param w Wood price of the building.
     * @param res_base Reference to the resource base.
     * @param interval Time interval for the building.
     * @param people Number of residents in the Halupa.
     */
    CHalupa(int g, int s, int w, CResources & res_base, Uint32 interval, int people);

    /**
     * @brief Destructor of the Halupa building. Removes one population
     */
    ~CHalupa() override;

    /**
     * @brief Update the resources of the Halupa building.
     */
    void update_resources() override;
};

/**
 * @brief Class for the Lumber building.
 */
class CLumber : public ABuilding // 1
{
public:
    static const int type = 1;

    /**
     * @brief Constructor for CLumber class.
     * @param g Gold price of the building.
     * @param s Stone price of the building.
     * @param w Wood price of the building.
     * @param res_base Reference to the resource base.
     * @param interval Time interval for the building.
     */
    CLumber(int g, int s, int w, CResources & res_base, Uint32 interval);

    /**
     * @brief Update the resources of the Lumber building.
     */
    void update_resources() override;
};

/**
 * @brief Class for the Mine building.
 */
class CMine : public ABuilding // 2
{
    bool m_gold;
public:
    static const int type = 2;

    /**
     * @brief Constructor for CMine class.
     * @param g Gold price of the building.
     * @param s Stone price of the building.
     * @param w Wood price of the building.
     * @param res_base Reference to the resource base.
     * @param interval Time interval for the building.
     * @param gold Flag indicating if the Mine produces gold or stone.
     */
    CMine(int g, int s, int w, CResources & res_base, Uint32 interval, bool gold);

    /**
     * @brief Update the resources of the Mine building.
     */
    void update_resources() override;
};


/**
 * @brief Class for the Fisher building.
 */
class CFisher : public ABuilding // 3
{
public:
    static const int type = 3;

    /**
     * @brief Constructor for CFisher class.
     * @param g Gold price of the building.
     * @param s Stone price of the building.
     * @param w Wood price of the building.
     * @param res_base Reference to the resource base.
     * @param interval Time interval for the building.
     */
    CFisher(int g, int s, int w, CResources & res_base, Uint32 interval);

    /**
     * @brief Update the resources of the Fisher building.
     */
    void update_resources() override;
};

/**
 * @brief Class for the Farm building.
 */
class CFarm : public ABuilding // 4
{
public:
    static const int type = 4;

    /**
     * @brief Constructor for CFarm class.
     * @param g Gold price of the building.
     * @param s Stone price of the building.
     * @param w Wood price of the building.
     * @param res_base Reference to the resource base.
     * @param interval Time interval for the building.
     */
    CFarm(int g, int s, int w, CResources & res_base, Uint32 interval);

    /**
     * @brief Update the resources of the Farm building.
     */
    void update_resources() override;
};
//----------------------------------------------------------------------------------------------------

/**
 * @brief Class for the building map. Stores a map of ABuildings with ids of each building as key
 */
class CBuildingMap
{
    std::map<int, std::shared_ptr<ABuilding>> m_building_map;
public:
    static int m_max_id;

    /**
     * @brief Add a building to the building map.
     * @tparam T Type of the building.
     * @param building Shared pointer to the building object.
     * @return The ID of the added building.
     */
    template <class T>
    int add_building(std::shared_ptr<T> building)
    {
        m_building_map.insert({m_max_id, building});
        return m_max_id++;    
    }
    template <class T>

    /**
     * @brief Add a building to the building map with a specified ID.
     * @tparam T Type of the building.
     * @param id The ID of the building.
     * @param building Shared pointer to the building object.
     * @return The ID of the added building.
     */
    int add_building_id(int id, std::shared_ptr<T> building)
    {
        m_building_map.insert({id, building});
        if (id > m_max_id) m_max_id = id + 1;
        return m_max_id;    
    }

    /**
     * @brief Remove a building from the building map.
     * @param id The ID of the building to be removed.
     */
    void remove_building(int id);

    /**
     * @brief Clear the building map.
     */
    void clear();
};