#pragma once
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>
#include <memory>
#include "Buildings.hpp"
#include "Map.hpp"
#include "Events.hpp"

class CRenderWindow
{
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
public:
    CRenderWindow(const char * title, int w, int h);
    ~CRenderWindow();
    SDL_Texture * loadTexture(const char * file_path);
    void clearScreen();
    void render(CHouse & h);
    void display();
    void saveWorld();
    void loadWorld();
};

class CPlayerState {
    int score;
    int resourceA;
    int resourceB;
public:
    CPlayerState();
    ~CPlayerState();
    void savestate();
    void loadstate();
};

class CGameManager {
    CPlayerState m_state;
    CRenderWindow m_windowobj;
    CTilemap m_tilemap;
    std::vector<CBuilding> m_buildings;
    std::vector<std::unique_ptr<IEvent>> m_events;
public:
    CGameManager();
    ~CGameManager();
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();
    void addBuilding();
    void removeBuilding();
    void updateGameState();
    void handleInput();
    void handleEvents();
    void renderGame();
    void saveGame();
    void loadGame();
};  