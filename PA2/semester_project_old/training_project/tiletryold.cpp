#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;

const int MAP_SIZE = 20;
const int INIT_X = 608;
const int INIT_Y = 0;
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;
const int TILE_SPRITE_AMOUNT = 9;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

class CTile
{
    int m_type;
    int m_resource;
public:
    CTile() : m_type(1), m_resource(0) {}
    CTile(int t, int r) : m_type(t), m_resource(r) {}
    int get_type() {return m_type;}
    int get_resource() {return m_resource;}
    void set_resource(int x) {m_resource = x;}
};

class CTileMap
{
    SDL_Texture * m_texture_set;                // set of textures for tilemap
    CTile m_tilemap[MAP_SIZE][MAP_SIZE];        // set of tiled objects
    SDL_Rect m_tilerect[MAP_SIZE][MAP_SIZE];    // dst_rect
    SDL_Rect m_tilesprites[TILE_SPRITE_AMOUNT]; // src_recr
    int m_hover_x;  // currently hovering tile x
    int m_hover_y;  // currently hovering tile y
public:
    CTileMap(SDL_Renderer * rend, const char * texture_src) : m_hover_x(-1), m_hover_y(-1)
    {
        SDL_Surface * tilemap_surface = SDL_LoadBMP(texture_src);
        // Create texture from a surface
        m_texture_set = SDL_CreateTextureFromSurface(rend, tilemap_surface);
        // Textures loaded to GPU, so we no longer need surface
        SDL_FreeSurface(tilemap_surface);

        // Populating a map with tiles
        for (int x = 0; x < MAP_SIZE; ++x)
        {
            for (int y = 0; y < MAP_SIZE; ++y)
            {
                m_tilemap[x][y] = CTile(rand() % 4 + 1, 0);
            }
        }
        // Putting resources
        for (int x = 0; x < MAP_SIZE; ++x)
        {
            for (int y = 0; y < MAP_SIZE; ++y)
            {
                if (m_tilemap[x][y].get_type() == 1)
                    m_tilemap[x][y].set_resource(rand() % 5);
            }
        }

        int init_x = INIT_X;
        int init_y = INIT_Y;
        for (int y = 0; y < 20; ++y)
        {
            for (int x = 0; x < 20; ++x)
            {
                m_tilerect[x][y].x = init_x + x * (TILE_WIDTH / 2);
                m_tilerect[x][y].y = init_y + x * (TILE_HEIGHT / 2);
                m_tilerect[x][y].w = TILE_WIDTH;
                m_tilerect[x][y].h = TILE_HEIGHT;
            }
            init_x -= (TILE_WIDTH / 2);
            init_y += (TILE_HEIGHT / 2);
        }

        m_tilesprites[0].x = m_tilesprites[2].x = 128;
        m_tilesprites[1].x = m_tilesprites[3].x = 192;
        m_tilesprites[0].y = m_tilesprites[1].y = m_tilesprites[4].y = m_tilesprites[5].x = m_tilesprites[7].x = 0;
        m_tilesprites[2].y = m_tilesprites[3].y = m_tilesprites[5].y = m_tilesprites[6].y = 32;
        m_tilesprites[4].x = m_tilesprites[6].x = m_tilesprites[8].x = m_tilesprites[7].y = m_tilesprites[8].y = 64;

        for (int i = 0; i < TILE_SPRITE_AMOUNT; ++i)
        {
            m_tilesprites[i].h = TILE_HEIGHT;
            m_tilesprites[i].w = TILE_WIDTH;
            if (i == 7 || i == 8)
                m_tilesprites[i].h *= 2;
        }
    }

    void displayMap(SDL_Renderer * rend)
    {
        // Loop for tiles
        for (int x = 0; x < MAP_SIZE; ++x)
        {
            for (int y= 0; y < MAP_SIZE; ++y)
            {
                switch (m_tilemap[x][y].get_type())
                {
                    case 1:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[0], &m_tilerect[x][y]);
                        break;
                    case 2:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[1], &m_tilerect[x][y]);
                        break;
                    case 3:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[2], &m_tilerect[x][y]);
                        break;
                    case 4:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[3], &m_tilerect[x][y]);
                        break;
                }
            }
        }
        // Loop for resources
        for (int x = 0; x < MAP_SIZE; ++x)
        {
            for (int y= 0; y < MAP_SIZE; ++y)
            {
                SDL_Rect tmp = m_tilerect[x][y];
                tmp.y = m_tilerect[x][y].y - 32;
                tmp.h = m_tilerect[x][y].h * 2;
                switch (m_tilemap[x][y].get_resource())
                {
                    case 0:
                        break;
                    case 1:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[5], &m_tilerect[x][y]);
                        break;
                    case 2:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[6], &m_tilerect[x][y]);
                        break;
                    case 3:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[7], &tmp);
                        break;
                    case 4:
                        SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[8], &tmp);
                        break;
                }
            }
        }
        if (m_hover_x >= 0 && m_hover_x < 20 && m_hover_y >= 0 && m_hover_y < 20)
        {
            SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[4], &m_tilerect[m_hover_x][m_hover_y]);
        }
        // Display what has been rendered
        SDL_RenderPresent(rend);
    }

    void set_hover_tile(int x, int y)
    {
        m_hover_x = x;
        m_hover_y = y;
        cout << "Type: " << m_tilemap[x][y].get_type() << endl;
        cout << "Resource: " << m_tilemap[x][y].get_resource() << endl;
        // if (x > 20 || y > 20 || x < 0 || y < 0) return;
        // SDL_RenderCopy(rend, m_texture_set, &m_tilesprites[4], &m_tilerect[x][y]);
        // SDL_RenderPresent(rend);
    }

    void get_cell_coord(int & Wx, int & Wy)
    {
        int x, y;
		SDL_GetMouseState(&x, &y);

        int cell_x = (x - INIT_X) / 64;
        if (x - INIT_X < 0) --cell_x;
        int cell_y = (y - INIT_Y) / 32;
        if (y - INIT_Y < 0) --cell_y;
        
        Wx = cell_y + cell_x;
        Wy = cell_y - cell_x;
        cout << "Wx = " << Wx << " Wy = " << Wy << endl;

        int offx = (x - INIT_X) % 64;
        offx < 0 ? offx = 64 + offx : offx;
        int offy = (y - INIT_Y) % 32;
        offy < 0 ? offy = 32 + offy : offy;

        cout << "OffX = " << offx << " offY = " << offy << endl;

        bool right = false;
        bool down = false;

        if (offx > 32)
        {
            offx = 64 - offx;
            right = true;
        }
        if (offy > 16)
        {
            offy = 32 - offy;
            down = true;
        }

        if (offx < 32 - 2 * offy)
        {
            if (!right && !down) Wx -= 1;
            else if (right && !down) Wy -= 1;
            else if (!right && down) Wy += 1;
            else Wx += 1;
        }
    } 
    void set_res_to_tile(int x, int y, int res)
    {
        m_tilemap[x][y].set_resource(res);
    }
};

int main(int argc, char * argv[])
{
    SDL_Window * window = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Fail on initializing SDL: " << SDL_GetError() << endl;
    }

    window = SDL_CreateWindow("Tilemap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    CTileMap gameMap(renderer, "images/textures.bmp");
    //CButtonList button_list(renderer, "images/pixil-frame-0_5_.bmp");

    //Infinite game loop
    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (event.type == SDL_MOUSEMOTION)
            {
                int Wx, Wy = -1;
                //int Bx, By = -1;
                gameMap.get_cell_coord(Wx, Wy); 
                gameMap.set_hover_tile(Wx, Wy);

                // button_list.get_button_coord(Bx, By);
                // button_list.set_hover_tile(Bx, By);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int Wx, Wy = -1;
                gameMap.get_cell_coord(Wx, Wy);
                gameMap.set_res_to_tile(Wx, Wy, 0);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Delay(20);

        gameMap.displayMap(renderer);
    }
}