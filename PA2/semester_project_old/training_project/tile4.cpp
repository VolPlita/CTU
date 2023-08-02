#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional> // Not needed now

#include "Text.hpp"

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

//-------------------------------------------------------------------------------------------------------------------------------------------------
class CResources // ADD PROTECTION FROM OVERFLOW
{
    map<string, int> m_resources;
public:
    CResources()
    {
        m_resources.insert({"Gold", 0});
        m_resources.insert({"Stone", 0});
        m_resources.insert({"Wood", 0});
        m_resources.insert({"Bread", 0});
        m_resources.insert({"Fish", 0});
        m_resources.insert({"Population", 0});
    }
    CResources(int g, int s, int w, int b, int f, int p)
    {
        m_resources.insert({"Gold", g});
        m_resources.insert({"Stone", s});
        m_resources.insert({"Wood", w});
        m_resources.insert({"Bread", b});
        m_resources.insert({"Fish", f});
        m_resources.insert({"Population", p});
    }
    void add_resource(const string & res, int amount)
    {
        if (m_resources.find(res) != m_resources.end())
            m_resources[res] += amount;
    }
    void set_resource(const string & res, int amount)
    {
        if (m_resources.find(res) != m_resources.end())
            m_resources[res] = amount;
    }
    void remove_resource(const string & res, int amount)
    {
        if (m_resources.find(res) != m_resources.end())
            m_resources[res] -= amount;
    }
    int get_resource(const string & res)
    {
        return m_resources.at(res);
    }
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct CTimer
{

};
//-------------------------------------------------------------------------------------------------------------------------------------------------
class CResourceText
{
    CResources * m_base;
    SDL_Renderer * renderer;
    CText m_gold;
    CText m_gold_amount;
    CText m_stone;
    CText m_stone_amount;
    CText m_wood;
    CText m_wood_amount;
    CText m_bread;
    CText m_bread_amount;
    CText m_fish;
    CText m_fish_amount;
    CText m_population;
    CText m_population_amount;
public:
    CResourceText(CResources * base, SDL_Renderer * rend) : m_base(base), renderer(rend),
    m_gold(renderer, TILE_WIDTH, 0, 29 * 5, 64, "Gold:"),
    m_stone(renderer, TILE_WIDTH + 29 * 5 + 2 * 29, 0, 29 * 6, 64, "Stone:"),
    m_wood(renderer, TILE_WIDTH + 29 * 5 + 29 * 6 + 4 * 29, 0, 29 * 5, 64, "Wood:"),
    m_bread(renderer, TILE_WIDTH + 2 * 29 * 5 + 29 * 6 + 6 * 29, 0, 29 * 6, 64, "Bread:"),
    m_fish(renderer, TILE_WIDTH + 2 * 29 * 5 + 2 * 29 * 6 + 8 * 29, 0, 29 * 5, 64, "Fish:"),
    m_population(renderer, TILE_WIDTH + 3 * 29 * 5 + 2 * 29 * 6 + 10 * 29, 0, 29 * 11, 64, "Population:"),
    m_gold_amount(renderer, TILE_WIDTH + 29 * 5, 0, 29, 64, to_string(m_base->get_resource("Gold"))),
    m_stone_amount(renderer, TILE_WIDTH + 29 * 5 + 2 * 29 + 29 * 6, 0, 29, 64, to_string(m_base->get_resource("Stone"))),
    m_wood_amount(renderer, TILE_WIDTH + 29 * 5 + 29 * 6 + 4 * 29 + 29 * 5, 0, 29, 64, to_string(m_base->get_resource("Wood"))),
    m_bread_amount(renderer, TILE_WIDTH + 2 * 29 * 5 + 29 * 6 + 6 * 29 + 29 * 6, 0, 29, 64, to_string(m_base->get_resource("Bread"))),
    m_fish_amount(renderer, TILE_WIDTH + 2 * 29 * 5 + 2 * 29 * 6 + 8 * 29 + 29 * 5, 0, 29, 64, to_string(m_base->get_resource("Fish"))),
    m_population_amount(renderer, TILE_WIDTH + 3 * 29 * 5 + 2 * 29 * 6 + 10 * 29 + 29 * 11, 0, 29, 64, to_string(m_base->get_resource("Population")))
    {}
    void render_text()
    {
        m_gold.render();
        m_gold_amount.render();
        m_stone.render();
        m_stone_amount.render();
        m_wood.render();
        m_wood_amount.render();
        m_bread.render();
        m_bread_amount.render();
        m_fish.render();
        m_fish_amount.render();
        m_population.render();
        m_population_amount.render();
    }
    void render_numbers()
    {
        m_gold_amount.change_text(to_string(m_base->get_resource("Gold")));
        m_gold_amount.render();
        m_stone_amount.change_text(to_string(m_base->get_resource("Stone")));
        m_stone_amount.render();
        m_wood_amount.change_text(to_string(m_base->get_resource("Wood")));
        m_wood_amount.render();
        m_bread_amount.change_text(to_string(m_base->get_resource("Bread")));
        m_bread_amount.render();
        m_fish_amount.change_text(to_string(m_base->get_resource("Fish")));
        m_fish_amount.render();
        m_population_amount.change_text(to_string(m_base->get_resource("Population")));
        m_population_amount.render();
    }
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
Uint32 callback(Uint32 interval, void * param);
class ABuilding
{
protected:
    int m_price_gold;
    int m_price_stone;
    int m_price_wood;
    CResources & m_resource_base;
    SDL_TimerID m_timer;
    Uint32 m_interval;
public:
    ABuilding(int g, int s, int w, CResources & res_base, Uint32 interval) : 
    m_price_gold(g), m_price_stone(s), m_price_wood(w), m_resource_base(res_base), m_interval(interval)
    {
        m_resource_base.remove_resource("Gold", m_price_gold);
        m_resource_base.remove_resource("Stone", m_price_stone);
        m_resource_base.remove_resource("Wood", m_price_wood);

        auto callmebabe = new std::function<void()>([&]() {
            this->update_resources();
        });
        m_timer = SDL_AddTimer(m_interval, callback, this);
    }
    ~ABuilding()
    {
        SDL_RemoveTimer(m_timer);
    }
    virtual void update_resources() 
    {
        cout << "I am here" << endl;
    }
    void reset_timer()
    {
        SDL_RemoveTimer(m_timer);
        m_timer = SDL_AddTimer(m_interval, callback, this);
    }
};
class CHalupa : public ABuilding // 0
{
    int m_residents_amount;
public: 
    static const int type = 0;
    CHalupa(int g, int s, int w, CResources & res_base, Uint32 interval, int people) : ABuilding(g, s, w, res_base, interval), m_residents_amount(people) 
    {
        m_resource_base.add_resource("Population", 1);
    }
    void update_resources() override {cout << "Halupa!!!" << endl;}
};
class CLumber : public ABuilding // 1
{
public:
    static const int type = 1;
    CLumber(int g, int s, int w, CResources & res_base, Uint32 interval) : ABuilding(g, s, w, res_base, interval) {}
    void update_resources() override
    {
        m_resource_base.add_resource("Wood", 5);
        cout << "Lumber!!!" << endl;
    }
};
class CMine : public ABuilding // 2
{
    bool m_gold;
public:
    static const int type = 2;
    CMine(int g, int s, int w, CResources & res_base, Uint32 interval, bool gold) : ABuilding(g, s, w, res_base, interval), m_gold(gold) {}
    void update_resources() override
    {
        if (m_gold)
            m_resource_base.add_resource("Gold", 3);
        else
            m_resource_base.add_resource("Stone", 5);

        cout << "Mine!!!" << endl;
    }
};
class CFisher : public ABuilding // 3
{
public:
    static const int type = 3;
    CFisher(int g, int s, int w, CResources & res_base, Uint32 interval) : ABuilding(g, s, w, res_base, interval) {}
    void update_resources() override
    {
        m_resource_base.add_resource("Fish", 5);
        cout << "Fisher!!!" << endl;
    }
};
class CFarm : public ABuilding // 4
{
public:
    static const int type = 4;
    CFarm(int g, int s, int w, CResources & res_base, Uint32 interval) : ABuilding(g, s, w, res_base, interval) {}
    void update_resources() override
    {
        m_resource_base.add_resource("Bread", 5);
        cout << "Farm!!!" << endl;
    }
};

class CBuildingMap
{
    map<int, shared_ptr<ABuilding>> m_building_map;
public:
    static int m_max_id;
    //CBuildingMap() {}
    ~CBuildingMap()
    {
        // Reset pointers
        //for (<int, shared_ptr<ABuilding>> : m_building_map)
    }
    template <class T>
    int add_building(shared_ptr<T> building)
    {
        m_building_map.insert({m_max_id, building});
        return m_max_id++;    
    }
    void remove_building(int id)
    {
        m_building_map[id].reset();
        m_building_map.erase(id);
    }
};
int CBuildingMap::m_max_id = 0;
//---
Uint32 callback(Uint32 interval, void * param)
{
    //cout << "Hahahaah!!!!" << endl;
    //((std::function<void()>*)param)->operator()();
    ((ABuilding*)param)->update_resources();
    ((ABuilding*)param)->reset_timer();
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------
class CTile
{
    int m_type;
    int m_resource;
	int m_building;
    int m_building_id;
public:
    CTile() : m_type(0), m_resource(-1), m_building(-1) , m_building_id(-1) {}
    CTile(int t, int r) : m_type(t), m_resource(r), m_building(-1), m_building_id(-1) {}
    CTile(int t, int r, int b) : m_type(t), m_resource(r), m_building(b), m_building_id(-1) {}
    CTile(int t, int r, int b, int bid) : m_type(t), m_resource(r), m_building(b), m_building_id(bid) {}
    int get_type() const {return m_type;}
    int get_resource() const {return m_resource;}
    void set_resource(int x) {m_resource = x;}
    int get_building() const {return m_building;}
    int get_building_id() const {return m_building_id;}
    void set_building(int x) {m_building = x;}
    void set_building_id(int id) {m_building_id = id;}
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
	virtual void get_cell_coord(int & Wx, int & Wy, int x, int y) const = 0;
};

class CTileMap : public AMap
{
    vector<vector<CTile>> m_map;     			// set of tiled objects
    vector<vector<SDL_Rect>> m_dst_rect;        // set of destination rectangles
    vector<SDL_Rect> m_sprites;                 // set of source rectangles
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
            //cout << "T: " << m_map[m_hover_x][m_hover_y].get_type() << " B: " << m_map[m_hover_x][m_hover_y].get_building() << " R: " << m_map[m_hover_x][m_hover_y].get_resource() << endl;
            SDL_RenderCopy(rend, m_texture_set, &m_sprites[0], &m_dst_rect[m_hover_x][m_hover_y]);
        }
    }

    void get_cell_coord(int & Wx, int & Wy, int x, int y) const override
    {
        int lwx, lwy;

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
    void build(int x, int y, int building, CBuildingMap & buildmap, CResources & res_base)
    {
        // MAKE MORE ADVANCED CHECK!!!
        //cout << "try to build" << endl;
        if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y && m_map[x][y].get_building() == -1 &&
            m_map[x][y].get_resource() == -1 && (m_map[x][y].get_type() == 0 || m_map[x][y].get_type() == 1 || m_map[x][y].get_type() == 3))
        {
            m_map[x][y].set_building(building);
            switch(building)
            {
                // ADD SETTING UP PARAMETERS FOR BUILDINGS
                case 0:
                    m_map[x][y].set_building_id(buildmap.add_building(make_shared<CHalupa>(1, 1, 1, res_base, 10 * 1000, 1)));
                    break;
                case 1:
                    m_map[x][y].set_building_id(buildmap.add_building(make_shared<CLumber>(1, 1, 1, res_base, 10 * 1000)));
                    break;
                case 2:
                    m_map[x][y].set_building_id(buildmap.add_building(make_shared<CMine>(1, 1, 1, res_base, 10 * 1000, false)));
                    break;
                case 3:
                    m_map[x][y].set_building_id(buildmap.add_building(make_shared<CFisher>(1, 1, 1, res_base, 10 * 1000)));
                    break;
                case 4:
                    m_map[x][y].set_building_id(buildmap.add_building(make_shared<CFarm>(1, 1, 1, res_base, 10 * 1000)));
                    break;
            }
            //buildmap.add_building();
        }
        else if (building == -1 && m_map[x][y].get_building() != -1)
        {
            buildmap.remove_building(m_map[x][y].get_building_id());
            m_map[x][y].set_building(-1);
            m_map[x][y].set_building_id(-1);
        }
    }
};

class CButton
{
    SDL_Rect m_sprite;
    SDL_Rect m_dst_rect;
public:
    CButton(int sx, int sy, int dx, int dy) 
    {
        m_sprite.h = m_sprite.w = m_dst_rect.h = m_dst_rect.w = 64;
        m_sprite.x = sx;
        m_sprite.y = sy;
        m_dst_rect.x = dx;
        m_dst_rect.y = dy;
    }
    void render(SDL_Renderer * rend, SDL_Texture * texture_set) const
    {
        SDL_RenderCopy(rend, texture_set, &m_sprite, &m_dst_rect);
    }
    SDL_Rect const & get_dst() const {return m_dst_rect;}
};

class CButtonList
{
    SDL_Texture * m_texture_set;
    SDL_Rect m_hover_sprite;
    SDL_Rect m_select_sprite;
    vector<CButton> m_button_list;
    int m_selected;
    int m_hovering;
public:
    CButtonList(SDL_Renderer * rend, const char * texture_src) : m_selected(-1) , m_hovering(-1)
    {
        SDL_Surface * tilemap_surface = SDL_LoadBMP(texture_src);
        m_texture_set = SDL_CreateTextureFromSurface(rend, tilemap_surface);
        SDL_FreeSurface(tilemap_surface); 

        m_hover_sprite.h = m_hover_sprite.w = m_select_sprite.h = m_select_sprite.w = m_select_sprite.y = 64;
        m_hover_sprite.x = m_hover_sprite.y = m_select_sprite.x = 0;
    }
    void add_button(int sx, int sy, int dx, int dy) 
    {
        m_button_list.emplace_back(sx, sy, dx, dy);
    }
    void set_selected(int x) {m_selected = x;}
    void set_hovering(int x) {m_hovering = x;}
    void render(SDL_Renderer * rend) const 
    {
        for (int i = 0; i < 4; ++i)
        {
            m_button_list[i].render(rend, m_texture_set);
        }
        if (m_selected == 1 || m_selected > 3)
        {
            for (int i = 4, l = static_cast<int>(m_button_list.size()); i < l; ++i)
            {
                m_button_list[i].render(rend, m_texture_set);
            }
        }
        if (m_hovering >= 0 && m_hovering < static_cast<int>(m_button_list.size()))
            SDL_RenderCopy(rend, m_texture_set, &m_hover_sprite, &m_button_list[m_hovering].get_dst());
        if (m_selected >= 0 && m_selected < static_cast<int>(m_button_list.size()))
            SDL_RenderCopy(rend, m_texture_set, &m_select_sprite, &m_button_list[m_selected].get_dst());
    }
    int get_selected() const {return m_selected;}
    void init_list()
    {
        this->add_button(64, 0, 0, 0);          // Settings - 0
        this->add_button(128, 0, 0, 64);        // Build - 1
        this->add_button(192, 0, 0, 128);       // Demolish - 2
        this->add_button(64, 64, 0, 192);       // Move - 3
        this->add_button(128, 64, 64, 64);      // House - 4
        this->add_button(192, 64, 64, 128);     // Mine - 5
        this->add_button(0, 128, 64, 192);      // Fisher - 6
        this->add_button(64, 128, 64, 256);     // Farm - 7
        this->add_button(128, 128, 64, 320);    // Lumber - 8
    }

    int get_hovering_index(int x, int y) const
    {
        if (m_selected == 1 || m_selected > 3)
        {
            if (x < 64 && y < 256) return y / 64;
            else if (x < 128 && y < 384 && x > 64 && y > 64) return y / 64 + 3;
            else return -1;
        }
        else 
        {
            if (x < 64 && y < 256) return y / 64;
            else return -1;
        }
    }
};
//-----------------------------------------------------------------------------------------------------------------------------------


// Uint32 callback( Uint32 interval, void* param )
// {
// 	//Print callback message
// 	printf( "Callback called back with message: %s\n", reinterpret_cast<char*>(param) );

// 	return 0;
// }
//-----------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    SDL_Window * window = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        cout << "Fail on initializing SDL: " << SDL_GetError() << endl;
    }

    window = SDL_CreateWindow("Super-puper city-building game 3000", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    CTileMap gameMap(renderer, "images/textures.bmp", MAP_SIZE, MAP_SIZE);
    CButtonList buttonList(renderer, "images/buttons.bmp");
    buttonList.init_list();
    CBuildingMap buildingMap;
    CResources resourceList(0, 10, 10, 10, 0, 0);

    CResourceText resourceBar(&resourceList, renderer);
    //Infinite game loop
    // To render before I need new renderer!!!!!!!!!!!!!!
    bool isRunning = true;
    while (isRunning)
    {
        int Wx, Wy = -1; // Currently hovering cell in world coordinates
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (event.type == SDL_MOUSEMOTION)
            {
                // RETHINK
                int Mx, My;
                SDL_GetMouseState(&Mx, &My);
                gameMap.get_cell_coord(Wx, Wy, Mx, My); 
                gameMap.set_hover_tile(Wx, Wy);

                buttonList.set_hovering(buttonList.get_hovering_index(Mx, My));
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                // 1) Get mouse location
                int Mx, My;
                SDL_GetMouseState(&Mx, &My);
                // 2) See at which part of the screen is the mouse
                if  (Mx <= 128 && My <= 384)    // Button part
                {
                    int index = buttonList.get_hovering_index(Mx, My);
                    if (index == buttonList.get_selected())
                        buttonList.set_selected(-1);
                    else
                        buttonList.set_selected(index);
                }
                else                            // Other / Map
                {
                    gameMap.get_cell_coord(Wx, Wy, Mx, My);
                    cout << "Wx = " << Wx << " Wy = " << Wy << endl;
                    switch (buttonList.get_selected())
                    {
                        case 2:
                            // Demolish
                            gameMap.build(Wx, Wy, -1, buildingMap, resourceList);
                            break;
                        case 4:
                            // House
                            gameMap.build(Wx, Wy, 0, buildingMap, resourceList);
                            break;
                        case 5:
                            // Mine
                            gameMap.build(Wx, Wy, 2, buildingMap, resourceList);
                            break;
                        case 6:
                            // Fisher
                            gameMap.build(Wx, Wy, 3, buildingMap, resourceList);
                            break;
                        case 7:
                            // Farm
                            gameMap.build(Wx, Wy, 4, buildingMap, resourceList);
                            break;
                        case 8:
                            // Lumber
                            gameMap.build(Wx, Wy, 1, buildingMap, resourceList);
                            break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Delay(20);
        // render map after each cycle
        gameMap.render(renderer);
        buttonList.render(renderer);
        cout << resourceList.get_resource("Gold") << endl;
        cout << resourceList.get_resource("Stone") << endl;
        cout << resourceList.get_resource("Wood") << endl;
        cout << resourceList.get_resource("Bread") << endl;
        cout << resourceList.get_resource("Fish") << endl;
        cout << resourceList.get_resource("Population") << endl;
        cout << "---------------------------------------" << endl;
        resourceBar.render_text();
        resourceBar.render_numbers();
        // Display what has been rendered to a screen
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
}

// Buildings:
// 0 - house;
// 1 - lumber;
// 2 - mine;
// 3 - fisher;
// 4 - farm;