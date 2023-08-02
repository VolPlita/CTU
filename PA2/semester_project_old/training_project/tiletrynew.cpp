#include <iostream>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 832; // 640 + 96 * 2

const int MAP_SIZE = 20;
const int INIT_X = 608;
const int INIT_Y = 96; // 64 + 32
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;
const int TILE_SPRITE_AMOUNT = 9;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

class COnMapObject
{
public:
	int m_type;
	COnMapObject() : m_type(1) {}
	COnMapObject(int t) : m_type(t) {}
	int get_type() {return m_type;}
};

class CTile : public COnMapObject
{
    int m_resource;
	//int m_building;
public:
    CTile() : COnMapObject(), m_resource(0) {}
    CTile(int t, int r) : COnMapObject(t), m_resource(r) {}
    int get_resource() {return m_resource;}
    void set_resource(int x) {m_resource = x;}
};

template <class T> // using keyword class for backward compatibility
class CTileMap
{
    SDL_Texture * m_texture_set;            // set of textures for tilemap
    vector<vector<T>> m_map;     			// set of tiled objects
    vector<vector<SDL_Rect>> m_dst_rect;    // set of destination rectangles
    vector<SDL_Rect> m_sprites;             // set of source rectangles
    // Size of tilemap
    int m_len_x;
    int m_len_y;
	int m_sprite_amount;
    // Coordinates of currently hovering tile
    int m_hover_x;
    int m_hover_y;
public:
	CTileMap(SDL_Renderer * rend, const char * texture_src, vector<vector<T>> & map_src, vector<vector<SDL_Rect>> & rct_src, vector<SDL_Rect> & sprites) 
	: m_hover_x(-1), m_hover_y(-1)
	{
		// 1) Creating a texture from a surface
		SDL_Surface * tilemap_surface = SDL_LoadBMP(texture_src);
        m_texture_set = SDL_CreateTextureFromSurface(rend, tilemap_surface);
        SDL_FreeSurface(tilemap_surface); 										// Textures loaded to GPU, so we no longer need surface

		// 2) Populating map with tiles (copy from somewhere) &&
		// 3) Setting destination rectangles (copy from somewhere)   
		// 	  Check passed map for correctness of size
		m_map = map_src;
		m_dst_rect = rct_src;
		m_len_y = map_src.size();
		if (m_len_y < 1 || rct_src.size() != static_cast<size_t>(m_len_y)) throw logic_error("Unacceptable map size.");
		else 
		{
			m_len_x = map_src[0].size();
			for (int i = 0; i < m_len_y; ++i)
			{
				if (map_src[i].size() != static_cast<size_t>(m_len_x) || rct_src[i].size() != static_cast<size_t>(m_len_x)) throw logic_error("Unacceptable map size.");
				else 
				{
					m_map[i] = map_src[i];
					m_dst_rect[i] = rct_src[i];
				}
			}
		}

		// 4) Setting sprites (copy from somewhere)
		m_sprites = sprites;
		m_sprite_amount = m_sprites.size();
	}
	// CTileMap<T>& operator = (CTileMap<T> & another)
	// {
	// 	m_texture_set = another.m_texture_set;
	// 	m_map = another.m_map;
	// 	m_dst_rect = another.m_dst_rect;
	// 	for(int i = 0, l = m_dst_rect.size(); i < l; ++i)
	// 	{
	// 		m_dst_rect[i] = another.m_dst_rect[i];
	// 		m_map[i] = another.m_map[i];
	// 	}
	// 	m_len_x = another.m_len_x;
	// 	m_len_y = another.m_len_y;
	// 	m_sprites = another.sprites;
	// 	m_sprite_amount = another.m_sprite_amount;
	// 	m_hover_x = another.m_hover_x;
	// 	m_hover_y = another.m_hover_y;
	// }

	bool display(SDL_Renderer * rend) // False means failure to display tile
	{
		// Loop for tiles
        for (int x = 0; x < m_len_x; ++x)
        {
            for (int y = 0; y < m_len_y; ++y)
            {
				int sprite_index = m_map[x][y].get_type();
				if (sprite_index >= m_sprite_amount) return false;
				SDL_RenderCopy(rend, m_texture_set, &m_sprites[sprite_index], &m_dst_rect[x][y]);
            }
        }

		// Loop for resources if they exist
        for (int x = 0; x < m_len_x; ++x)
        {
            for (int y = 0; y < m_len_y; ++y)
            {
                SDL_Rect tmp = m_dst_rect[x][y];
                tmp.y = m_dst_rect[x][y].y - 32;
                tmp.h = m_dst_rect[x][y].h * 2;

				int sprite_index = m_map[x][y].get_resource();
				if (sprite_index >= m_sprite_amount) return false;
				if (sprite_index == 3 || sprite_index == 4)
					SDL_RenderCopy(rend, m_texture_set, &m_sprites[sprite_index + 4], &tmp);
				else
					SDL_RenderCopy(rend, m_texture_set, &m_sprites[sprite_index + 4], &m_dst_rect[x][y]);
            }
        }

		// Loop for buildings if they exist
		// TODO

		if (m_hover_x >= 0 && m_hover_x < m_len_x && m_hover_y >= 0 && m_hover_y < m_len_y)
        {
            SDL_RenderCopy(rend, m_texture_set, &m_sprites[0], &m_dst_rect[m_hover_y][m_hover_x]);
        }
        // Display what has been rendered
        SDL_RenderPresent(rend);
		return true;
	}

	void set_hover_tile(int x, int y)
    {
		if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y)
		{
        	m_hover_x = x;
        	m_hover_y = y;
			cout << "Type: " << m_map[x][y].get_type() << endl;
        	cout << "Resource: " << m_map[x][y].get_resource() << endl;
		}
    }
	void get_cell_coord(int & Wx, int & Wy)
    {
		int lwx, lwy;
        int x, y;
		SDL_GetMouseState(&x, &y);

        int cell_x = (x - INIT_X) / 64;
        if (x - INIT_X < 0) --cell_x;
        int cell_y = (y - INIT_Y) / 32;
        if (y - INIT_Y < 0) --cell_y;
        
        lwx = cell_y + cell_x;
        lwy = cell_y - cell_x;
        cout << "Wx = " << lwx << " Wy = " << lwy << endl;

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
            if (!right && !down) lwx -= 1;
            else if (right && !down) lwy -= 1;
            else if (!right && down) lwy += 1;
            else lwx += 1;
        }

		if (lwx >= 0 && lwx < m_len_x && lwy >= 0 && lwy < m_len_y)
		{
			Wx = lwx;
			Wy = lwy;
		}
    } 
    void set_res_to_tile(int x, int y, int res)
    {
		// MAKE MORE SECURE!!!!!
        m_map[x][y].set_resource(res);
    }
};

CTileMap<CTile> rand_gameMap(SDL_Renderer * rend, const char * texture_src, int size)
{
	// Create random tilemap
	vector<vector<CTile>> tilemap;
    for (int x = 0; x < size; ++x)
    {
        vector<CTile> row;
        for (int y = 0; y < size; ++y)
        {
            row.emplace_back(CTile(rand() % 4 + 1, 0));
            //tilemap[x][y] = CTile(rand() % 4 + 1, 0);
        }
        tilemap.push_back(row);
    }
    // Putting resources
    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            if (tilemap[x][y].get_type() == 1)
                tilemap[x][y].set_resource(rand() % 5);
        }
    }

	// Setting destination rectangles
	vector<vector<SDL_Rect>> rectangles;
	int init_x = INIT_X;
    int init_y = INIT_Y;
    for (int y = 0; y < size; ++y)
    {
        vector<SDL_Rect> row;
        for (int x = 0; x < size; ++x)
        {
            SDL_Rect rectangle;
            rectangle.x = init_x + x * (TILE_WIDTH / 2);
            rectangle.y = init_y + x * (TILE_HEIGHT / 2);
            rectangle.w = TILE_WIDTH;
            rectangle.h = TILE_HEIGHT;
            row.emplace_back(rectangle);
        }
		rectangles.push_back(row);
        init_x -= (TILE_WIDTH / 2);
        init_y += (TILE_HEIGHT / 2);
    }

	// Setting sprites
	// MAKE MORE AUTOMATIC
	vector<SDL_Rect> sprites;
	for (int i = 0; i < 9; ++i)
	{
		SDL_Rect tmp;
		sprites.push_back(tmp);
		sprites[i].h = TILE_HEIGHT;
        sprites[i].w = TILE_WIDTH;
        if (i == 7 || i == 8)
            sprites[i].h *= 2;
	}
	sprites[5].x = sprites[7].x = sprites[0].y = sprites[1].y = sprites[2].y = 0;
	sprites[5].y = sprites[6].y = sprites[3].y = sprites[4].y = 32;
	sprites[0].x = sprites[6].x = sprites[8].x = sprites[7].y = sprites[8].y = 64;
	sprites[1].x = sprites[3].x = 128;
	sprites[2].x = sprites[4].x = 192;

	return CTileMap<CTile>(rend, texture_src, tilemap, rectangles, sprites);
}


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

    CTileMap<CTile> gameMap = rand_gameMap(renderer, "images/textures.bmp", 20);
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
                int Wx = -1;
				int Wy = -1;
                //int Bx, By = -1;
                gameMap.get_cell_coord(Wx, Wy); 
                gameMap.set_hover_tile(Wx, Wy);

                // button_list.get_button_coord(Bx, By);
                // button_list.set_hover_tile(Bx, By);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int Wx = -1;
				int Wy = -1;
                gameMap.get_cell_coord(Wx, Wy);
                gameMap.set_res_to_tile(Wx, Wy, 0);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Delay(20);

        gameMap.display(renderer);
    }
}