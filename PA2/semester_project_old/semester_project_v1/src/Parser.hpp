#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#include "define.hpp"
#include "Tiles.hpp"

class CParser
{
public:
    CWorldData read_map(const std::string & filename);
    void save_map(CTileMap & map, CResources & res) const;
};