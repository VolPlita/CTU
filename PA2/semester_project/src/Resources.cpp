#include "Resources.hpp"

CResources::CResources()
{
    std::cout << "1st constr of SResources" << std::endl;
    m_resources.insert({"Gold", 5});
    m_resources.insert({"Stone", 5});
    m_resources.insert({"Wood", 5});
    m_resources.insert({"Bread", 5});
    m_resources.insert({"Fish", 5});
    m_resources.insert({"Population", 0});
}
CResources::CResources(int g, int s, int w, int b, int f, int p)
{
    std::cout << "2nd constr of SResources" << std::endl;
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

/**
 * @brief Constructor for CResourceText with a base CResources object.
 * @param base Pointer to the base CResources object.
 * @param rend The SDL renderer for rendering the text.
 */
CResourceText::CResourceText(CResources * base, SDL_Renderer * rend) : m_base(base),
    m_text(rend, TILE_WIDTH, 0, "Gold:        Stone:        Wood:        Bread:        Fish:        Population:        ", 48),
    m_gold_amount(rend, TILE_WIDTH + 24 * 4, 0, std::to_string(m_base->get_resource("Gold")), 48),
    m_stone_amount(rend, TILE_WIDTH + 24 * 12, 0, std::to_string(m_base->get_resource("Stone")), 48),
    m_wood_amount(rend, TILE_WIDTH + 24 * 19, 0, std::to_string(m_base->get_resource("Wood")), 48),
    m_bread_amount(rend, TILE_WIDTH + 24 * 26, 0, std::to_string(m_base->get_resource("Bread")), 48),
    m_fish_amount(rend, TILE_WIDTH + 24 * 33, 0, std::to_string(m_base->get_resource("Fish")), 48),
    m_population_amount(rend, TILE_WIDTH + 24 * 45, 0, std::to_string(m_base->get_resource("Population")), 48)
    {std::cout << "Constructor of CResourceText" << std::endl;}

CResourceText::~CResourceText() {std::cout << "DESTRUCTOR CResourceText" << std::endl; m_base = nullptr;}

void CResourceText::change_base(CResources * base)
{
    m_base = base;
}
void CResourceText::render_text(SDL_Renderer * rend)
{
    m_text.render(rend);
    m_gold_amount.render(rend);
    m_stone_amount.render(rend);
    m_wood_amount.render(rend);
    m_bread_amount.render(rend);
    m_fish_amount.render(rend);
    m_population_amount.render(rend);
}
void CResourceText::render_numbers(SDL_Renderer * rend)
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