#pragma once
#include <vector>
#include <SDL2/SDL.h>


/**
 * @class CButton
 * @brief Represents a button in the user interface.
 *
 * The CButton class represents a button in the user interface. It encapsulates the sprite and destination rectangle of the button.
 */
class CButton
{
    SDL_Rect m_sprite;    ///< The sprite rectangle representing the button.
    SDL_Rect m_dst_rect;  ///< The destination rectangle where the button will be rendered.
public:

    /**
     * @brief Constructor for CButton class.
     * @param sx The x-coordinate of the sprite rectangle.
     * @param sy The y-coordinate of the sprite rectangle.
     * @param dx The x-coordinate of the destination rectangle.
     * @param dy The y-coordinate of the destination rectangle.
     *
     * Creates a button with the specified sprite and destination rectangles.
     */
    CButton(int sx, int sy, int dx, int dy);

    /**
     * @brief Renders the button on the renderer using the provided texture set.
     * @param rend The SDL_Renderer used for rendering.
     * @param texture_set The texture set used for rendering the button.
     */
    void render(SDL_Renderer * rend, SDL_Texture * texture_set) const;

    /**
     * @brief Retrieves the destination rectangle of the button.
     * @return The destination rectangle of the button.
     */
    [[nodiscard]] const SDL_Rect & get_dst() const;
};

/**
 * @class CButtonList
 * @brief Represents a list of buttons in the user interface.
 *
 * The CButtonList class represents a list of buttons in the user interface. It manages the buttons, their selection and hovering states.
 */
class CButtonList
{
    SDL_Texture * m_texture_set;            ///< The texture set used for rendering the buttons.
    SDL_Rect m_hover_sprite{};                ///< The sprite rectangle for the hovering state.
    SDL_Rect m_select_sprite{};               ///< The sprite rectangle for the selected state.
    std::vector<CButton> m_button_list;     ///< The list of buttons in the button list.
    int m_selected;                         ///< The index of the currently selected button.
    int m_hovering;                         ///< The index of the currently hovering button.
public:

    /**
     * @brief Constructor for CButtonList class.
     * @param rend The SDL_Renderer used for rendering.
     * @param texture_src The filename of the texture for the buttons.
     *
     * Creates a button list with the specified renderer and texture source.
     */
    CButtonList(SDL_Renderer * rend, const char * texture_src);

    /**
     * @brief Destructor for CButtonList class.
     *
     * Cleans up the resources used by the button list.
     */
    ~CButtonList();

    /**
     * @brief Adds a button to the button list.
     * @param sx The x-coordinate of the sprite rectangle of the button.
     * @param sy The y-coordinate of the sprite rectangle of the button.
     * @param dx The x-coordinate of the destination rectangle of the button.
     * @param dy The y-coordinate of the destination rectangle of the button.
     */
    void add_button(int sx, int sy, int dx, int dy);

     /**
     * @brief Sets the selected button in the button list.
     * @param x The index of the selected button.
     */
    void set_selected(int x);

    /**
     * @brief Sets the hovering button in the button list.
     * @param x The index of the hovering button.
     */
    void set_hovering(int x);

    /**
     * @brief Renders the button list on the renderer.
     * @param rend The SDL_Renderer used for rendering.
     */
    void render(SDL_Renderer * rend) const;

    /**
     * @brief Retrieves the index of the selected button.
     * @return The index of the selected button.
     */
    [[nodiscard]] int get_selected() const;

    /**
     * @brief Retrieves the index of the hovering button at the specified coordinates.
     * @param x The x-coordinate of the mouse pointer.
     * @param y The y-coordinate of the mouse pointer.
     * @return The index of the hovering button, or -1 if no button is hovering at the specified coordinates.
     */
    [[nodiscard]] int get_hovering_index(int x, int y) const;

    /**
     * @brief Initializes the button list.
     *
     * Initializes the button list by setting the selected and hovering buttons to -1.
     */
    void init_list();
};