#include "Tiles.hpp"

CTile::CTile() : m_type(0), m_resource(-1), m_building(-1) , m_building_id(-1) {}
CTile::CTile(int t, int r) : m_type(t), m_resource(r), m_building(-1), m_building_id(-1) {}
CTile::CTile(int t, int r, int b) : m_type(t), m_resource(r), m_building(b), m_building_id(-1) {}
CTile::CTile(int t, int r, int b, int bid) : m_type(t), m_resource(r), m_building(b), m_building_id(bid) {}
int CTile::get_type() const {return m_type;}
int CTile::get_resource() const {return m_resource;}
void CTile::set_resource(int x) {m_resource = x;}
int CTile::get_building() const {return m_building;}
int CTile::get_building_id() const {return m_building_id;}
void CTile::set_building(int x) {m_building = x;}
void CTile::set_building_id(int id) {m_building_id = id;}
//-------------------------------------------------------------------------------------------------------------
AMap::AMap(SDL_Renderer *& rend, const char * texture_src, int lenx, int leny) : m_len_x(lenx), m_len_y(leny), m_hover_x(-1), m_hover_y(-1)
{
	// Creating a texture from a surface
	SDL_Surface * tilemap_surface = SDL_LoadBMP(texture_src);
    m_texture_set = SDL_CreateTextureFromSurface(rend, tilemap_surface);
    SDL_FreeSurface(tilemap_surface); 										// Textures loaded to GPU, so we no longer need surface
}
AMap::~AMap()
{
    SDL_DestroyTexture(m_texture_set);
    m_texture_set = nullptr;
}

void AMap::set_hover_tile(int x, int y)
{
    if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y)
	{
    	m_hover_x = x;
    	m_hover_y = y;
	}
    else m_hover_x = m_hover_y = -1;
}
//-------------------------------------------------------------------------------------------------------------
CTileMap::CTileMap(SDL_Renderer *& rend, const char * texture_src, int lenx, int leny) : AMap(rend, texture_src, lenx, leny)
{
    //Setting destination rectangles
    for (int y = 0; y < m_len_y; ++y)
    {
        std::vector<SDL_Rect> row;
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

CTileMap::~CTileMap()
{
    SDL_DestroyTexture(m_texture_set);
    m_texture_set = nullptr;
}

void CTileMap::render(SDL_Renderer *& rend) const
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

void CTileMap::populate(CWorldData data)
{
    // Pre-clear the map
    for (size_t i = 0, l = m_map.size(); i < l; i++)
    {
        m_map[i].clear();
    }
    m_map.clear();

    for (int x = 0; x < m_len_x; ++x)
    {
        std::vector<CTile> row;
        for (int y = 0; y < m_len_y; ++y)
        {
            row.emplace_back(CTile(data.m_types[y + x * MAP_SIZE],
                                   data.m_resources[y + x * MAP_SIZE],
                                   data.m_buildings[y + x * MAP_SIZE],
                                   data.m_building_ids[y + x * MAP_SIZE]));
        }
        m_map.push_back(row);
    }
}

void CTileMap::populate()
{
    srand(time(0));
    // Populating a map with tiles
    for (int x = 0; x < m_len_x; ++x)
    {
        std::vector<CTile> row;
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
}

std::vector<std::vector<CTile>> * CTileMap::access_map()
{
    return &m_map;
}   

void CTileMap::get_cell_coord(int & Wx, int & Wy, int x, int y) const
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

void CTileMap::set_res_to_tile(int x, int y, int res)
{
    if (x >= 0 && x < m_len_x && y >= 0 && y < m_len_y)
        m_map[x][y].set_resource(res);
}

void CTileMap::build(int x, int y, int building, CBuildingMap & buildmap, CResources & res_base)
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
                m_map[x][y].set_building_id(buildmap.add_building(std::make_shared<CHalupa>(1, 1, 1, res_base, 10 * 1000, 1)));
                break;
            case 1:
                m_map[x][y].set_building_id(buildmap.add_building(std::make_shared<CLumber>(1, 1, 1, res_base, 10 * 1000)));
                break;
            case 2:
                m_map[x][y].set_building_id(buildmap.add_building(std::make_shared<CMine>(1, 1, 1, res_base, 10 * 1000, false)));
                break;
            case 3:
                m_map[x][y].set_building_id(buildmap.add_building(std::make_shared<CFisher>(1, 1, 1, res_base, 10 * 1000)));
                break;
            case 4:
                m_map[x][y].set_building_id(buildmap.add_building(std::make_shared<CFarm>(1, 1, 1, res_base, 10 * 1000)));
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
