#include "Parser.hpp"

CWorldData CParser::read_map(const std::string & filename)
{
    CWorldData ret;
    std::ifstream input;
    input.open(filename);
    if (!input.is_open())
    {
        std::cout << "Could not open reading file." << std::endl;
        //return;
    }
    std::string line;
    if (getline(input, line, '{'))
    {
        getline(input, line, '[');
        if (line.find("\"types\" :") != std::string::npos)
        {
            int i = 0;
            while(true)
            {
                getline(input, line, ',');
                if (line.find("]") != std::string::npos)
                {
                    ret.m_types[i] = stoi(line);
                    break;
                }
                ret.m_types[i++] = stoi(line);
            }
        }
        getline(input, line, '[');
        if (line.find("\"resources\" :") != std::string::npos)
        {
            int i = 0;
            while(true)
            {
                getline(input, line, ',');
                if (line.find("]") != std::string::npos)
                {
                    ret.m_resources[i] = stoi(line);
                    break;
                }
                ret.m_resources[i++] = stoi(line);
            }
        }
        getline(input, line, '[');
        if (line.find("\"buildings\" :") != std::string::npos)
        {
            int i = 0;
            while(true)
            {
                getline(input, line, ',');
                if (line.find("]") != std::string::npos)
                {
                    ret.m_buildings[i] = stoi(line);
                    break;
                }
                ret.m_buildings[i++] = stoi(line);
            }
        }
        getline(input, line, '[');
        if (line.find("\"building_ids\" :") != std::string::npos)
        {
            int i = 0;
            while(true)
            {
                getline(input, line, ',');
                if (line.find("]") != std::string::npos)
                {
                    ret.m_building_ids[i] = stoi(line);
                    break;
                }
                ret.m_building_ids[i++] = stoi(line);
            }
        }
    
    }

    getline(input, line, ':');
    if (line.find("\"Gold\"") != std::string::npos)
    {
        getline(input, line, ',');
        ret.m_res_bar[0] = stoi(line);
    }
    getline(input, line, ':');
    if (line.find("\"Stone\"") != std::string::npos)
    {
        getline(input, line, ',');
        ret.m_res_bar[1] = stoi(line);
    }
    getline(input, line, ':');
    if (line.find("\"Wood\"") != std::string::npos)
    {
        getline(input, line, ',');
        ret.m_res_bar[2] = stoi(line);
    }
    getline(input, line, ':');
    if (line.find("\"Bread\"") != std::string::npos)
    {
        getline(input, line, ',');
        ret.m_res_bar[3] = stoi(line);
    }
    getline(input, line, ':');
    if (line.find("\"Fish\"") != std::string::npos)
    {
        getline(input, line, ',');
        ret.m_res_bar[4] = stoi(line);
    }
    getline(input, line, ':');
    if (line.find("\"Population\"") != std::string::npos)
    {
        getline(input, line, '}');
        ret.m_res_bar[5] = stoi(line);
    }

    input.close();
    return ret;
}

void CParser::save_map(CTileMap & map, CResources & res) const
{
    CWorldData save_world;
    std::vector<std::vector<CTile>> * map_arr = map.access_map();
    for (size_t i = 0, l = map_arr->size(); i < l; i++)
    {
        for (size_t j = 0; j < l; j++)
        {
            save_world.m_types[i * l + j] = map_arr->at(i).at(j).get_type();
            save_world.m_resources[i * l + j] = map_arr->at(i).at(j).get_resource();
            save_world.m_buildings[i * l + j] = map_arr->at(i).at(j).get_building();
            save_world.m_building_ids[i * l + j] = map_arr->at(i).at(j).get_building_id();
        }
    }

    time_t t = std::time(nullptr);
    tm tms = *std::localtime(&t);

    std::ostringstream filename;
    filename << "worlds/save_from_" << std::put_time(&tms, "%d:%m:%Y_%H:%M:%S") << ".json";
    // std::cout << filename.str() << std::endl;
    std::ofstream output(filename.str());
    if (!output.is_open())
    {
        std::cout << "Error opening file." << std::endl;
    }

    output << "{\n";
    output << "\t\"types\" : [\n";
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        output << "\t\t\t\t";
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            i == MAP_SIZE - 1 && j == MAP_SIZE - 1 ?
                output << save_world.m_types[i * MAP_SIZE + j] << " ],\n" :
                output << save_world.m_types[i * MAP_SIZE + j] << ", ";
        }
        output << "\n";
    }

    output << "\t\"resources\" : [\n";
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        output << "\t\t\t\t\t";
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            i == MAP_SIZE - 1 && j == MAP_SIZE - 1 ?
                output << save_world.m_resources[i * MAP_SIZE + j] << " ],\n" :
                output << save_world.m_resources[i * MAP_SIZE + j] << ", ";
        }
        output << "\n";
    }

    output << "\t\"buildings\" : [\n";
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        output << "\t\t\t\t\t";
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            i == MAP_SIZE - 1 && j == MAP_SIZE - 1 ?
                output << save_world.m_buildings[i * MAP_SIZE + j] << " ],\n" :
                output << save_world.m_buildings[i * MAP_SIZE + j] << ", ";
        }
        output << "\n";
    }

    output << "\t\"building_ids\" : [\n";
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        output << "\t\t\t\t\t";
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            i == MAP_SIZE - 1 && j == MAP_SIZE - 1 ?
                output << save_world.m_building_ids[i * MAP_SIZE + j] << " ],\n" :
                output << save_world.m_building_ids[i * MAP_SIZE + j] << ", ";
        }
        output << "\n";
    }

    output << "\t\"Gold\" : " << res.get_resource("Gold") << ",\n";
    output << "\t\"Stone\" : " << res.get_resource("Stone") << ",\n";
    output << "\t\"Wood\" : " << res.get_resource("Wood") << ",\n";
    output << "\t\"Bread\" : " << res.get_resource("Bread") << ",\n";
    output << "\t\"Fish\" : " << res.get_resource("Fish") << ",\n";
    output << "\t\"Population\" : " << res.get_resource("Population") << "\n";

    output << "}" << std::endl;
    
    output.close();
}