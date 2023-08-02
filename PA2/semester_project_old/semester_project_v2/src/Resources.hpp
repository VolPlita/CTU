#pragma once
#include <map>
#include "define.hpp"
#include "Text.hpp"

class CResources // ADD PROTECTION FROM OVERFLOW
{
    std::map<std::string, int> m_resources;
public:
    CResources();
    CResources(int g, int s, int w, int b, int f, int p);
    void add_resource(const std::string & res, int amount);
    void set_resource(const std::string & res, int amount);
    void remove_resource(const std::string & res, int amount);
    int get_resource(const std::string & res);
};

class CResourceText
{
    CResources * m_base;
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
    CResourceText(CResources * base, SDL_Renderer *& rend);
    CResourceText(SDL_Renderer *& rend);
    ~CResourceText();
    void change_base(CResources * base);
    void render_text(SDL_Renderer *& rend);
    void render_numbers(SDL_Renderer *& rend);
};