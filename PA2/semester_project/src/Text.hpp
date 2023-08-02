#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


/**
 * @class CText
 * @brief Represents a text element to be rendered on the screen.
 */
class CText
{
    // static bool is_initialized;     ///< Indicates if the TTF library is initialized. --OLD
    SDL_Texture * m_texture;        ///< The texture used to render the text. 
    TTF_Font * m_font;              ///< The font used for rendering the text. 
    SDL_Rect m_dst_rect{};            ///< The destination rectangle for rendering the text.
    std::string m_text;             ///< The text string to be rendered.
    SDL_Color m_color;
public:

    /**
     * @brief Default constructor for CText.
     */
    // CText();

    /**
     * @brief Constructor for CText.
     * @param rend The SDL renderer.
     * @param x The x-coordinate of the text position.
     * @param y The y-coordinate of the text position.
     * @param text The text string to be rendered.
     */
    CText(SDL_Renderer * rend, int x, int y, std::string text, int font_size);

    /**
     * @brief Destructor for CText.
     */
    ~CText();

    /**
     * @brief Changes the text string to be rendered.
     * @param rend The SDL renderer.
     * @param text The new text string.
     */
    void change_text(SDL_Renderer * rend, const std::string & text);

    /**
     * @brief Changes the color of the text.
     * @param rend The SDL renderer.
     * @param color The new color for the text.
     */
    void change_color(SDL_Renderer * rend, SDL_Color color);

    /**
     * @brief Gets the x-coordinate of the text position.
     * @return The x-coordinate.
     */
    [[nodiscard]] int get_x() const;

    /**
     * @brief Gets the y-coordinate of the text position.
     * @return The y-coordinate.
     */
    [[nodiscard]] int get_y() const;

    /**
     * @brief Gets the width of the rendered text.
     * @return The width of the text.
     */
    [[nodiscard]] int get_w() const;

    /**
     * @brief Gets the height of the rendered text.
     * @return The height of the text.
     */
    [[nodiscard]] int get_h() const;

    /**
     * @brief Gets the text string.
     * @return The text string.
     */
    std::string get_string();

    /**
     * @brief Renders the text on the screen.
     * @param rend The SDL renderer.
     */
    void render(SDL_Renderer * rend);
};