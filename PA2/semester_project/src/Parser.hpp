#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#include "define.hpp"
#include "Tiles.hpp"


/**
 * @class CParser
 * @brief Class for reading and saving map data from/to a file.
 */
class CParser
{
public:

    /**
     * @brief Reads map data from a file.
     * @param filename The name of the file to read.
     * @return The read CWorldData object.
     */
    static CWorldData read_map(const std::string & filename);

    /**
     * @brief Saves map data and resource information to a file.
     * @param map The CTileMap object containing the map data.
     * @param res The CResources object containing the resource information.
     */
    static void save_map(CTileMap & map, CResources & res) ;
};