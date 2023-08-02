#pragma once
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>
#include "Buildings.hpp"
#include "GameControll.hpp"

class CTile {
    double m_x;
    double m_y;
    SDL_Texture * m_texture;
    CBuilding * m_building;
public:
    CTile(double x, double y, SDL_Texture *texture);
    ~CTile();
    void setBuilding(CBuilding *building);
    CBuilding *getBuilding() const;
    double getX() const;
    double getY() const;
    SDL_Texture *getTexture() const;
    void render(CRenderWindow &window);
};

class CTilemap {
    std::vector<std::vector<CTile>> m_tiles;
    int m_tileSize;
    int m_width;
    int m_height;
public:
    CTilemap(int width, int height, int tileSize);
    ~CTilemap();
    void setTile(int x, int y, CTile *tile);
    CTile *getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    int getTileSize() const;
    void render(CRenderWindow &window);
};