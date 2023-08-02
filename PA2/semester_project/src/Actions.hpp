#pragma once
#include <SDL2/SDL.h>
#include "Tiles.hpp"

/**
 * @brief Callback function for the action timer. Function is called when timer of CAction instance reaches 0, it calls 
 * the empose_effect() function of respective child of CAction and resets the timer to repeat the action again in the same period of time
 * @param interval The interval in milliseconds.
 * @param param Pointer to the CAction object.
 * @return The callback return value.
 */
Uint32 callback_action(Uint32 interval, void * param);


/**
 * Base class for game actions. Contains references to CTileMap and CBuildingMap as actions might want to have effect on worldmap.
 * Also stores timer and interval, through which the action will be repeated
 */
class CAction
{
protected:
    CTileMap & m_map_ref;       ///< Reference to the tile map.
    CBuildingMap & m_build_ref; ///< Reference to the building map.
    SDL_TimerID m_timer;        ///< Timer ID.
    Uint32 m_interval;          ///< Time interval for the action.
public:
    /**
     * @brief Constructor for CAction class.
     * @param map Reference to the tile map.
     * @param build Reference to the building map.
     * @param interval Time interval for the action.
     */
    CAction(CTileMap & map, CBuildingMap & build, Uint32 interval);
    
    /**
     * @brief Destructor for CAction class.
     */
    virtual ~CAction();

    /**
     * @brief Reset the action timer.
     */
    void reset_timer();

    /**
     * @brief Apply the effect of the action.
     */
    virtual void empose_effect() const = 0;
};

/**
 *  Class for the flood action. Floods some random sand tiles and destroys buildings and resources located on this tiles. Bad for fishers.
 */
class CFlood : public CAction
{
public:
    /**
     * @brief Constructor for CFlood class.
     * @param map Reference to the tile map.
     * @param build Reference to the building map.
     * @param interval Time interval for the action.
     */
    CFlood(CTileMap & map, CBuildingMap & build, Uint32 interval);
    void empose_effect() const override;
};

/**
 *  Class for the tree growth action. Grows some trees on free grass and flower tiles.
 */
class CTreeGrowth : public CAction
{
public:

    /**
     * @brief Constructor for CTreeGrowth class.
     * @param map Reference to the tile map.
     * @param build Reference to the building map.
     * @param interval Time interval for the action.
     */
    CTreeGrowth(CTileMap & map, CBuildingMap & build, Uint32 interval);
    
    /**
     * @brief Apply the effect of the tree growth action.
     */
    void empose_effect() const override;
};