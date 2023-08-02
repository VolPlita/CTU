#include "Resources.hpp"

CResources::CResources()
{
    m_resources.insert({"Gold", 5});
    m_resources.insert({"Stone", 5});
    m_resources.insert({"Wood", 5});
    m_resources.insert({"Bread", 5});
    m_resources.insert({"Fish", 5});
    m_resources.insert({"Population", 0});
}
CResources::CResources(int g, int s, int w, int b, int f, int p)
{
    m_resources.insert({"Gold", g});
    m_resources.insert({"Stone", s});
    m_resources.insert({"Wood", w});
    m_resources.insert({"Bread", b});
    m_resources.insert({"Fish", f});
    m_resources.insert({"Population", p});
}
void CResources::add_resource(const std::string & res, int amount)
{
    if (m_resources.find(res) != m_resources.end())
        m_resources[res] += amount;
}
void CResources::set_resource(const std::string & res, int amount)
{
    if (m_resources.find(res) != m_resources.end())
        m_resources[res] = amount;
}
void CResources::remove_resource(const std::string & res, int amount)
{
    if (m_resources.find(res) != m_resources.end())
        m_resources[res] -= amount;
}
int CResources::get_resource(const std::string & res)
{
    return m_resources.at(res);
}
//--------------------------------------------------------------------
CResourceText::CResourceText(CResources * base, SDL_Renderer *& rend) : m_base(base),
    m_gold(rend, TILE_WIDTH, 0, "Gold:"),
    m_gold_amount(rend, TILE_WIDTH + 29 * 5, 0, std::to_string(m_base->get_resource("Gold"))),
    m_stone(rend, TILE_WIDTH + 29 * 5 + 2 * 29, 0, "Stone:"),
    m_stone_amount(rend, TILE_WIDTH + 29 * 5 + 2 * 29 + 29 * 6, 0, std::to_string(m_base->get_resource("Stone"))),
    m_wood(rend, TILE_WIDTH + 29 * 5 + 29 * 6 + 4 * 29, 0, "Wood:"),
    m_wood_amount(rend, TILE_WIDTH + 29 * 5 + 29 * 6 + 4 * 29 + 29 * 5, 0, std::to_string(m_base->get_resource("Wood"))),
    m_bread(rend, TILE_WIDTH + 2 * 29 * 5 + 29 * 6 + 6 * 29, 0, "Bread:"),
    m_bread_amount(rend, TILE_WIDTH + 2 * 29 * 5 + 29 * 6 + 6 * 29 + 29 * 6, 0, std::to_string(m_base->get_resource("Bread"))),
    m_fish(rend, TILE_WIDTH + 2 * 29 * 5 + 2 * 29 * 6 + 8 * 29, 0, "Fish:"),
    m_fish_amount(rend, TILE_WIDTH + 2 * 29 * 5 + 2 * 29 * 6 + 8 * 29 + 29 * 5, 0, std::to_string(m_base->get_resource("Fish"))),
    m_population(rend, TILE_WIDTH + 3 * 29 * 5 + 2 * 29 * 6 + 10 * 29, 0, "Population:"),
    m_population_amount(rend, TILE_WIDTH + 3 * 29 * 5 + 2 * 29 * 6 + 10 * 29 + 29 * 11, 0, std::to_string(m_base->get_resource("Population")))
    {}
CResourceText::CResourceText(SDL_Renderer *& rend) : m_base(nullptr),
    m_gold(rend, TILE_WIDTH, 0, "Gold:"),
    m_gold_amount(rend, TILE_WIDTH + 29 * 5, 0, "0"),
    m_stone(rend, TILE_WIDTH + 29 * 5 + 2 * 29, 0, "Stone:"),
    m_stone_amount(rend, TILE_WIDTH + 29 * 5 + 2 * 29 + 29 * 6, 0, "0"),
    m_wood(rend, TILE_WIDTH + 29 * 5 + 29 * 6 + 4 * 29, 0, "Wood:"),
    m_wood_amount(rend, TILE_WIDTH + 29 * 5 + 29 * 6 + 4 * 29 + 29 * 5, 0, "0"),
    m_bread(rend, TILE_WIDTH + 2 * 29 * 5 + 29 * 6 + 6 * 29, 0, "Bread:"),
    m_bread_amount(rend, TILE_WIDTH + 2 * 29 * 5 + 29 * 6 + 6 * 29 + 29 * 6, 0, "0"),
    m_fish(rend, TILE_WIDTH + 2 * 29 * 5 + 2 * 29 * 6 + 8 * 29, 0, "Fish:"),
    m_fish_amount(rend, TILE_WIDTH + 2 * 29 * 5 + 2 * 29 * 6 + 8 * 29 + 29 * 5, 0, "0"),
    m_population(rend, TILE_WIDTH + 3 * 29 * 5 + 2 * 29 * 6 + 10 * 29, 0, "Population:"),
    m_population_amount(rend, TILE_WIDTH + 3 * 29 * 5 + 2 * 29 * 6 + 10 * 29 + 29 * 11, 0, "0")
    {}
CResourceText::~CResourceText() {m_base = nullptr;}

void CResourceText::change_base(CResources * base)
{
    m_base = base;
}
void CResourceText::render_text(SDL_Renderer *& rend)
{
    m_gold.render(rend);
    m_gold_amount.render(rend);
    m_stone.render(rend);
    m_stone_amount.render(rend);
    m_wood.render(rend);
    m_wood_amount.render(rend);
    m_bread.render(rend);
    m_bread_amount.render(rend);
    m_fish.render(rend);
    m_fish_amount.render(rend);
    m_population.render(rend);
    m_population_amount.render(rend);
}
void CResourceText::render_numbers(SDL_Renderer *& rend)
{
    m_gold_amount.change_text(rend, std::to_string(m_base->get_resource("Gold")));
    m_gold_amount.render(rend);
    m_stone_amount.change_text(rend, std::to_string(m_base->get_resource("Stone")));
    m_stone_amount.render(rend);
    m_wood_amount.change_text(rend, std::to_string(m_base->get_resource("Wood")));
    m_wood_amount.render(rend);
    m_bread_amount.change_text(rend, std::to_string(m_base->get_resource("Bread")));
    m_bread_amount.render(rend);
    m_fish_amount.change_text(rend, std::to_string(m_base->get_resource("Fish")));
    m_fish_amount.render(rend);
    m_population_amount.change_text(rend, std::to_string(m_base->get_resource("Population")));
    m_population_amount.render(rend);
}