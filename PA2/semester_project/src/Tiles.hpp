#pragma once
#include <vector>
#include <SDL2/SDL.h>

#include "define.hpp"
#include "Buildings.hpp"

/**
 * @class CTile
 * @brief Represents a tile in the game. It has four parameters:
 * type, resourcem building type and building id
 */
class CTile
{
    int m_type;
    int m_resource;
	int m_building;
    int m_building_id;
public:
    CTile();
    CTile(int t, int r);
    CTile(int t, int r, int b);
    CTile(int t, int r, int b, int bid);
    [[nodiscard]] int get_type() const;
    void set_type(int x);
    [[nodiscard]] int get_resource() const;
    void set_resource(int x);
    [[nodiscard]] int get_building() const;
    [[nodiscard]] int get_building_id() const;
    void set_building(int x);
    void set_building_id(int id);
};
//-------------------------------------------------------------------------------------------------------------

/**
 * @struct CWorldData
 * @brief Represents world data. Used for saving and transfering information about tiles and resources
 */
struct CWorldData
{
    int m_types[MAP_SIZE * MAP_SIZE];
    int m_resources[MAP_SIZE * MAP_SIZE];
    int m_buildings[MAP_SIZE * MAP_SIZE];
    int m_building_ids[MAP_SIZE * MAP_SIZE];
    int m_res_bar[6];
};
//-------------------------------------------------------------------------------------------------------------

/**
 * @class AMap
 * @brief Abstract class representing a map. Saves set of textures used for the CTileMap
 */
class AMap
{
protected:
    SDL_Texture * m_texture_set;            // set of textures for tilemap
    // Size of tilemap
    int m_len_x;
    int m_len_y;
    // Coordinates of currently hovering tile
    int m_hover_x; ///< Coordinate x of currently hovering tile in World coordinate system
    int m_hover_y; ///< Coordinate y of currently hovering tile in World coordinate system
public:
	AMap(SDL_Renderer * rend, const char * texture_src, int lenx, int leny);
    ~AMap();
	void set_hover_tile(int x, int y);
    virtual void render(SDL_Renderer * rend) const = 0;
	virtual void get_cell_coord(int & Wx, int & Wy, int x, int y) const = 0;
};
//-------------------------------------------------------------------------------------------------------------

/**
 * @class CTileMap
 * @brief Represents a tile map.
 */
class CTileMap : public AMap
{
    std::vector<std::vector<CTile>> m_map;     			///< set of tiled objects
    std::vector<std::vector<SDL_Rect>> m_dst_rect;      ///< set of destination rectangles
    std::vector<SDL_Rect> m_sprites;                    ///< set of source rectangles
public:
    CTileMap(SDL_Renderer * rend, const char * texture_src, int lenx, int leny);
    ~CTileMap();

    void render(SDL_Renderer * rend) const override;
    void populate(CWorldData data);
    void populate();

    std::vector<std::vector<CTile>> * access_map();

    void get_cell_coord(int & Wx, int & Wy, int x, int y) const override;
    void set_res_to_tile(int x, int y, int res);
    void build(int x, int y, int building, CBuildingMap & buildmap, CResources & res_base);
    void remove_tree();
};
