#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

// enum Preset
// {
//     WIDTH = 1280,
//     HEIGHT = 720,
//     FPS = 60,
//     DISPLAY_OPTION = 0
// };

class File_Handler 
{
private:
    // Stage layout in integer array
    std::vector<std::vector<uint8_t>> stage_int;

    const char *config_path = "data/config.txt";

    // Data type string for parsing
    std::vector<std::string> data_type = 
    {
        "width",
        "height",
        "fps",
        "display option"
    };

    // Array for values (same order as data type for easier parsing)
    std::vector<uint16_t> values =
    {
        0,
        0,
        0,
        0
    };

public:
    // Checking if a line has a specific string
    bool compareString(std::string line, std::string str);

    // Remove a certain character in a string
    void removeChar(std::string& str, char ch);

    // Grab value from a data row
    void grabValue(std::string& row);

    // Function for reading game settings, configs
    void readConfig();
    
    // Function for reading stage layout in CSV form
    // Convert the block array from string to int and store in stage_int
    void readCSV(const char *path);

    // Getters
    uint16_t getValue(int i);
    std::vector<std::vector<uint8_t>> getStageInt();
};

#endif