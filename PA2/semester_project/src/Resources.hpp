#pragma once
#include <map>
#include "define.hpp"
#include "Text.hpp"

/**
 * @class CResources
 * @brief Represents a collection of resources.
 * 
 * This class manages different types of resources and their amounts.
 */
class CResources
{
    std::map<std::string, int> m_resources;
public:

    /**
     * @brief Default constructor for CResources.
     * 
     * Initializes the resources with default values.
     */
    CResources();

    /**
     * @brief Constructor for CResources.
     * @param g Initial amount of gold.
     * @param s Initial amount of stone.
     * @param w Initial amount of wood.
     * @param b Initial amount of bread.
     * @param f Initial amount of fish.
     * @param p Initial population count.
     */
    CResources(int g, int s, int w, int b, int f, int p);

    /**
     * @brief Adds a specified amount to a resource.
     * @param res The name of the resource.
     * @param amount The amount to be added.
     */
    void add_resource(const std::string & res, int amount);

    /**
     * @brief Sets the amount of a resource to a specified value.
     * @param res The name of the resource.
     * @param amount The new amount for the resource.
     */
    void set_resource(const std::string & res, int amount);

    /**
     * @brief Removes a specified amount from a resource.
     * @param res The name of the resource.
     * @param amount The amount to be removed.
     */
    void remove_resource(const std::string & res, int amount);

    /**
     * @brief Gets the current amount of a resource.
     * @param res The name of the resource.
     * @return The current amount of the resource.
     */
    int get_resource(const std::string & res);
};

/**
 * @class CResourceText
 * @brief Represents the text display for resources.
 * 
 * This class manages the rendering of resource information on the screen.
 */
class CResourceText
{
    CResources * m_base;
    CText m_text;
    CText m_gold_amount;
    CText m_stone_amount;
    CText m_wood_amount;
    CText m_bread_amount;
    CText m_fish_amount;
    CText m_population_amount;
public:

    /**
     * @brief Constructor for CResourceText with a base CResources object.
     * @param base Pointer to the base CResources object.
     * @param rend The SDL renderer for rendering the text.
     */
    CResourceText(CResources * base, SDL_Renderer * rend);

    /**
     * @brief Destructor for CResourceText.
     */
    ~CResourceText();

    /**
     * @brief Changes the base CResources object.
     * @param base Pointer to the new base CResources object.
     */
    void change_base(CResources * base);

     /**
     * @brief Renders the resource text on the screen.
     * @param rend The SDL renderer for rendering the text.
     */
    void render_text(SDL_Renderer * rend);

    /**
     * @brief Renders the resource numbers on the screen.
     * @param rend The SDL renderer for rendering the text.
     */
    void render_numbers(SDL_Renderer * rend);
};