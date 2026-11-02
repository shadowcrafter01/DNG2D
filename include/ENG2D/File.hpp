#ifndef ENG_FILE_HPP
#define ENG_FILE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

#include "ENG2D/Console.hpp"

using json = nlohmann::json;

namespace ENG
{

    class File
    {
    private:
        std::string path;
        std::ofstream fileOut;
        std::ifstream fileIn;
        std::vector<std::string> names;
        std::vector<std::string> values;
        json j;

    public:
        File(std::string path) : path{path}
        {
            fileIn.open(path);
            try
            {
                j = json::parse(fileIn);
            }
            catch (json::parse_error &e)
            {
                Console::LogError("Could not parse JSON", e.what());
            }
            fileIn.close();
        }

        template <class T>
        void readProperty(std::string name, T *value, T default_value)
        {
            if (!j[name.c_str()].empty())
            {
                *value = j[name.c_str()];
                Console::LogInfo("Read " + name + " -> " + std::to_string(*value) + " from file [" + path + "]", " -FILE : ");
                return;
            }
            Console::LogWarn("Tried to read null from JSON [" + path + "], initializing with and returning default value");
            writeProperty(name, default_value);
            *value = default_value;
            return;
        }

        template <class T>
        void writeProperty(std::string name, T value)
        {
            fileOut.open(path);
            j[name] = value;
            fileOut << j.dump();
            fileOut.close();
            Console::LogInfo("Write " + name + " <- " + std::to_string(value) + " to file [" + path + "]", " -FILE : ");
        }

        std::string GetPath()
        {
            return path;
        }
    };

};

#endif