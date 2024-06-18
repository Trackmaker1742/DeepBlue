#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

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

    // Return integer value in config
    int value(int iter, std::string row);

    // Grab value from a data row
    bool grabValue(std::string& row);

    // Function for reading stage layout in CSV form
    // Convert the block array from string to int and store in stage_int
    void readCSV();

    // Function for reading config file, game settings and stuff
    void readConfig();
    
    // Getters
    std::vector<std::vector<uint8_t>> getStageInt();
};

#endif