#include <iostream>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_WIDTH = 1536; // 1280 + 256
const int SCREEN_HEIGHT = 832; // 640 + 96 * 2

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;
const int MAP_SIZE = 20;
const int INIT_X = SCREEN_WIDTH / 2 - 32;
const int INIT_Y = TILE_HEIGHT + TILE_WIDTH; // 64 + 32
const int TILE_SPRITE_AMOUNT = 20;
const int BUTTON_SPRITE_AMOUNT = 8;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

class CTile
{
    int m_type;
    int m_resource;
	int m_building;
public:
    CTile() : m_type(0), m_resource(-1), m_building(-1) {}
    CTile(int t, int r) : m_type(t), m_resource(r), m_building(-1) {}
    CTile(int t, int r, int b) : m_type(t), m_resource(r), m_building(b) {}
    int get_type() const {return m_type;}
    int get_resource() const {return m_resource;}
    void set_resource(int x) {m_resource = x;}
    int get_building() const {return m_building;}
    void set_building(int x) {m_building = x;}
};

class AMap
{
protected:
    SDL_Texture * m_texture_set;            // set of textures for tilemap
    // Size of tilemap
    int m_len_x;
    int m_len_y;
    // Coordinates of currently hovering tile
    int m_hover_x;
    int m_hover_y;
public:
	AMap(SDL_Renderer * rend, const char * texture_src, int lenx, int leny) : m_len_x(lenx), m_len_y(leny), m_hover_x(-1), m_hover_y(-1)
	{
		// Creating a texture from a surface
		SDL_Surface * tilemap_surface = SDL_LoadBMP(texture_src);
        m_texture_set = SDL_CreateTextureFromSurface(rend, tilemap_surface);
        SDL_FreeSurface(tilemap_surface); 										// Textures loaded to GPU, so we no longer need surface
	}

	void set_hover_tile(int x, int y)
    {
        if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y)
		{
        	m_hover_x = x;
        	m_hover_y = y;
		}
        else m_hover_x = m_hover_y = -1;
    }

    virtual void render(SDL_Renderer * rend) const = 0;
	virtual void get_cell_coord(int & Wx, int & Wy) const = 0;
    // void set_res_to_tile(int x, int y, int res); - MOVE TO CHILD
};

class CTileMap : public AMap
{
    vector<vector<CTile>> m_map;     			// set of tiled objects
    vector<vector<SDL_Rect>> m_dst_rect;        // set of destination rectangles
    vector<SDL_Rect> m_sprites;                 // set of source rectangles
    //int m_sprite_amount;
public:
    CTileMap(SDL_Renderer * rend, const char * texture_src, int lenx, int leny) : AMap(rend, texture_src, lenx, leny)
    {
        // Populating a map with tiles
        for (int x = 0; x < m_len_x; ++x)
        {
            vector<CTile> row;
            for (int y = 0; y < m_len_y; ++y)
            {
                row.emplace_back(CTile(rand() % 4, -1, -1));
                //tilemap[x][y] = CTile(rand() % 4 + 1, 0);
            }
            m_map.push_back(row);
        }
        // Putting resources
        for (int x = 0; x < m_len_x; ++x)
        {
            for (int y = 0; y < m_len_y; ++y)
            {
                if (m_map[x][y].get_type() == 0)
                    m_map[x][y].set_resource(rand() % 5 - 1);
            }
        }
        //Setting destination rectangles
        for (int y = 0; y < m_len_y; ++y)
        {
            vector<SDL_Rect> row;
            for (int x = 0; x < m_len_x; ++x)
            {
                SDL_Rect rectangle;
                row.emplace_back(rectangle);
            }
	    	m_dst_rect.push_back(row);
        }
        int init_x = INIT_X;
        int init_y = INIT_Y;
        for (int y = 0; y < 20; ++y)
        {
            for (int x = 0; x < 20; ++x)
            {
                m_dst_rect[x][y].x = init_x + x * (TILE_WIDTH / 2);
                m_dst_rect[x][y].y = init_y + x * (TILE_HEIGHT / 2);
                m_dst_rect[x][y].w = TILE_WIDTH;
                m_dst_rect[x][y].h = TILE_HEIGHT;
            }
            init_x -= (TILE_WIDTH / 2);
            init_y += (TILE_HEIGHT / 2);
        }
        // Setting sprites for tiles and resources
        for (int i = 0; i < TILE_SPRITE_AMOUNT; ++i)
	    {
	    	SDL_Rect tmp;
	    	m_sprites.push_back(tmp);
	    	m_sprites[i].h = TILE_HEIGHT;
            m_sprites[i].w = TILE_WIDTH;
            if (i > 7)
                m_sprites[i].h *= 2;
	    }

        m_sprites[0].x = m_sprites[4].x = m_sprites[8].x = m_sprites[12].x = m_sprites[16].x = m_sprites[0].y = m_sprites[1].y = m_sprites[2].y = m_sprites[3].y = 0;
        m_sprites[4].y = m_sprites[5].y = m_sprites[6].y = m_sprites[7].y = 32;
        m_sprites[1].x = m_sprites[5].x = m_sprites[9].x = m_sprites[13].x = m_sprites[17].x = m_sprites[8].y = m_sprites[9].y = m_sprites[10].y = m_sprites[11].y = 64;
        m_sprites[2].x = m_sprites[6].x = m_sprites[10].x = m_sprites[14].x = m_sprites[18].x = m_sprites[12].y = m_sprites[13].y = m_sprites[14].y = m_sprites[15].y = 128;
        m_sprites[3].x = m_sprites[7].x = m_sprites[11].x = m_sprites[15].x = m_sprites[19].x = m_sprites[16].y = m_sprites[17].y = m_sprites[18].y = m_sprites[19].y = 192;
    }

    void render(SDL_Renderer * rend) const override
    {
        // Loop for tiles
        for (int x = 0; x < m_len_x; ++x)
        {
            for (int y = 0; y < m_len_y; ++y)
            {
                int index = m_map[x][y].get_type();
                if (index >= 0 && index < 4)
                {
                    if (index > 1)
                        SDL_RenderCopy(rend, m_texture_set, &m_sprites[index + 4], &m_dst_rect[x][y]);
                    else
                        SDL_RenderCopy(rend, m_texture_set, &m_sprites[index + 2], &m_dst_rect[x][y]);
                }
                // switch (m_map[x][y].get_type())
                // {
                //     case 1:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[0], &m_dst_rect[x][y]);
                //         break;
                //     case 2:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[1], &m_dst_rect[x][y]);
                //         break;
                //     case 3:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[2], &m_dst_rect[x][y]);
                //         break;
                //     case 4:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[3], &m_dst_rect[x][y]);
                //         break;
                // }
            }
        }
        // Loop for resources and buildings
        for (int x = 0; x < m_len_x; ++x)
        {
            for (int y = 0; y < m_len_y; ++y)
            {
                SDL_Rect tmp = m_dst_rect[x][y];
                tmp.y = m_dst_rect[x][y].y - 32;
                tmp.h = m_dst_rect[x][y].h * 2;
                int index = m_map[x][y].get_resource();
                if (index >= 0 && index < 4)
                {
                    if (index > 1)
                        SDL_RenderCopy(rend, m_texture_set, &m_sprites[index + 6], &tmp);
                    else 
                        SDL_RenderCopy(rend, m_texture_set, &m_sprites[index + 4], &m_dst_rect[x][y]);
                }
                // switch (m_map[x][y].get_resource())
                // {
                //     case 0:
                //         break;
                //     case 1:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[5], &m_dst_rect[x][y]);
                //         break;
                //     case 2:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[6], &m_dst_rect[x][y]);
                //         break;
                //     case 3:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[7], &tmp);
                //         break;
                //     case 4:
                //         SDL_RenderCopy(rend, m_texture_set, &m_sprites[8], &tmp);
                //         break;
                // }
                index = m_map[x][y].get_building();
                if (index >= 0 && index < 8)
                {
                    SDL_RenderCopy(rend, m_texture_set, &m_sprites[index + 12], &tmp);
                }
                // MAKE ALL INDEXES LIMITS NOT HARDCODED!!!
            }
        }
        if (m_hover_x >= 0 && m_hover_x < m_len_x && m_hover_y >= 0 && m_hover_y < m_len_y)
        {
            cout << "T: " << m_map[m_hover_x][m_hover_y].get_type() << " B: " << m_map[m_hover_x][m_hover_y].get_building() << " R: " << m_map[m_hover_x][m_hover_y].get_resource() << endl;
            SDL_RenderCopy(rend, m_texture_set, &m_sprites[0], &m_dst_rect[m_hover_x][m_hover_y]);
        }
    }

    void get_cell_coord(int & Wx, int & Wy) const override
    {
        int x, y, lwx, lwy;
		SDL_GetMouseState(&x, &y);

        int cell_x = (x - INIT_X) / 64;
        if (x - INIT_X < 0) --cell_x;
        int cell_y = (y - INIT_Y) / 32;
        if (y - INIT_Y < 0) --cell_y;
        
        lwx = cell_y + cell_x;
        lwy = cell_y - cell_x;
        //cout << "Wx = " << lwx << " Wy = " << lwy << endl;

        int offx = (x - INIT_X) % 64;
        offx < 0 ? offx = 64 + offx : offx;
        int offy = (y - INIT_Y) % 32;
        offy < 0 ? offy = 32 + offy : offy;

        //cout << "OffX = " << offx << " offY = " << offy << endl;

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
            if (!right && !down) lwx -= 1;
            else if (right && !down) lwy -= 1;
            else if (!right && down) lwy += 1;
            else lwx += 1;
        }
        Wx = lwx;
        Wy = lwy;
    } 
    void set_res_to_tile(int x, int y, int res)
    {
        if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y)
            m_map[x][y].set_resource(res);
    }
    void build(int x, int y, int building)
    {
        // MAKE MORE ADVANCED CHECK!!!
        if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y && 
            m_map[x][y].get_resource() == -1 && (m_map[x][y].get_type() == 0 || m_map[x][y].get_type() == 1 || m_map[x][y].get_type() == 3))
            m_map[x][y].set_building(building);
    }
};

class CButton
{
    int m_type;
    //bool is_selected;
public:
    CButton() : m_type(0) {}
    CButton(int t) : m_type(t) {}
    int get_type() const {return m_type;}
    //bool get_selected() const {return is_selected;}
    //void set_selected(bool a) {is_selected = a;}
};

class CButtonMap : public AMap
{
    int m_selected_x;
    int m_selected_y;
    vector<vector<CButton>> m_map;     			// set of tiled objects
    vector<vector<SDL_Rect>> m_dst_rect;        // set of destination rectangles
    vector<SDL_Rect> m_sprites;                 // set of source rectangles
public:
    CButtonMap(SDL_Renderer * rend, const char * texture_src, int lenx, int leny) : AMap(rend, texture_src, lenx, leny), m_selected_x(-1), m_selected_y(-1)
    {
        // Putting buttons at place
        for (int x = 0; x < m_len_x; ++x)
        {
            vector<CButton> row;
            for (int y = 1; y <= m_len_y; ++y)
            {
                row.emplace_back(y);
            }
            m_map.push_back(row);
        }
        // Creating destination rectangles
        for (int x = 0; x < m_len_x; ++x)
        {
            vector<SDL_Rect> col;
            for(int y = 0; y < m_len_y; ++y)
            {
                SDL_Rect rectangle;
                rectangle.x = x * TILE_WIDTH;
                rectangle.y = y * TILE_WIDTH;
                rectangle.h = rectangle.w = TILE_WIDTH;
                col.emplace_back(rectangle);
            }
            m_dst_rect.push_back(col);
        }
        // Setting sprites
        for (int i = 0; i < TILE_SPRITE_AMOUNT; ++i)
	    {
	    	SDL_Rect tmp;
	    	m_sprites.push_back(tmp);
            m_sprites[i].h = m_sprites[i].w = TILE_WIDTH;
	    }
        m_sprites[0].x = m_sprites[4].x = m_sprites[0].y = m_sprites[1].y = m_sprites[2].y = m_sprites[3].y = 0;
        m_sprites[1].x = m_sprites[5].x = m_sprites[4].y = m_sprites[5].y = m_sprites[6].y = m_sprites[7].y = 64;
        m_sprites[2].x = m_sprites[6].x = 128;
        m_sprites[3].x = m_sprites[7].x = 192;
    }
    // TYPE IS SAME AS INDEX HERE
    void render(SDL_Renderer * rend) const override
    {
        for (int x = 0; x < m_len_x; ++x)
        {
            for (int y = 0; y < m_len_y; ++y)
            {
                SDL_RenderCopy(rend, m_texture_set, &m_sprites[m_map[x][y].get_type()], &m_dst_rect[x][y]);
            }
        }
        if (m_hover_x >= 0 && m_hover_x < m_len_x && m_hover_y >= 0 && m_hover_y < m_len_y)
        {
            SDL_RenderCopy(rend, m_texture_set, &m_sprites[0], &m_dst_rect[m_hover_x][m_hover_y]);
        }
        if (m_selected_x >= 0 && m_selected_x < m_len_x && m_selected_y >= 0 && m_selected_y < m_len_y)
        {
            SDL_RenderCopy(rend, m_texture_set, &m_sprites[4], &m_dst_rect[m_selected_x][m_selected_y]);
        }
    }
    void get_cell_coord(int & Bx, int & By) const override
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x > TILE_WIDTH * m_len_x || y > TILE_WIDTH * m_len_y) 
        {
            Bx = By = -1;
            return;
        }
        Bx = x / TILE_WIDTH;
        By = y / TILE_WIDTH;
        cout << "Bx = " << Bx << " By = " << By << endl;
    }
    void set_selected(int x, int y)
    {
        // cout << x << " " << y << endl;
        if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y)
        {
            if (m_selected_x == x && m_selected_y == y)
                m_selected_x = m_selected_y = -1;
            else
            {
                m_selected_x = x;
                m_selected_y = y;
            }
        }
        // cout << "selX = " << m_selected_x << endl;
        // cout << "selY = " << m_selected_y << endl;
    }
    int get_sel_x() const {return m_selected_x;}
    int get_sel_y() const {return m_selected_y;}
};
//-----------------------------------------------------------------------------------------------------------------------------------
class ABuilding
{
protected:
    pair<int, int> m_coords;
    int m_resource_per_tic;
public:
    ABuilding(int res) : m_coords(make_pair(-1, -1)), m_resource_per_tic(res) {}
};
class CHalupa : public ABuilding
{
    int m_residents_amount;
public: 
    static const int type = 1;
    CHalupa(int people, int res) : ABuilding(res), m_residents_amount(people) {}
};
class CLumber : public ABuilding
{
    int m_var;
public:
    static const int type = 2;
    CLumber(int v, int r) : ABuilding(r), m_var(v) {}
};
class CMine : public ABuilding
{
    int m_var;
public:
    static const int type = 3;
    CMine(int v, int r) : ABuilding(r), m_var(v) {}
};
class CFisher : public ABuilding
{
    int m_var;
public:
    static const int type = 4;
    CFisher(int v, int r) : ABuilding(r), m_var(v) {}
};
class CFarm : public ABuilding
{
    int m_var;
public:
    static const int type = 5;
    CFarm(int v, int r) : ABuilding(r), m_var(v) {}
};
//-----------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    SDL_Window * window = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Fail on initializing SDL: " << SDL_GetError() << endl;
    }

    window = SDL_CreateWindow("Super-puper city-building game 3000", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    CTileMap gameMap(renderer, "images/textures.bmp", MAP_SIZE, MAP_SIZE);
    CButtonMap buttonList(renderer, "images/buttons.bmp", 1, 3);
    vector<shared_ptr<ABuilding>> buildingList;

    //Infinite game loop
    bool isRunning = true;
    while (isRunning)
    {
        int Wx, Wy = -1; // Currently hovering cell
        int Bx, By = -1; // Currently hovering button
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (event.type == SDL_MOUSEMOTION)
            {
                gameMap.get_cell_coord(Wx, Wy); 
                gameMap.set_hover_tile(Wx, Wy);

                buttonList.get_cell_coord(Bx, By);
                buttonList.set_hover_tile(Bx, By);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                gameMap.get_cell_coord(Wx, Wy); 
                //gameMap.set_res_to_tile(Wx, Wy, 0);

                buttonList.get_cell_coord(Bx, By);  
                buttonList.set_selected(Bx, By);

                // Make better choice of buttons
                if (buttonList.get_sel_x() == 0 && buttonList.get_sel_y() == 1)
                {
                    // Build
                    gameMap.build(Wx, Wy, 0);
                }
                else if (buttonList.get_sel_x() == 0 && buttonList.get_sel_y() == 2)
                {
                    // Demolish
                    gameMap.build(Wx, Wy, -1);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Delay(20);
        // render map after each cycle
        gameMap.render(renderer);
        buttonList.render(renderer);
        // Display what has been rendered to a screen
        SDL_RenderPresent(renderer);
    }
}