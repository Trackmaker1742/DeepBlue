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

    std::vector<std::string> config_array;

    std::string reso_str;

public:
    // Checking if a line has a specific string
    bool compareString(std::string line, std::string str);

    // Remove a certain character in a string
    void removeChar(std::string str, char ch);

    // Function for reading stage layout in CSV form
    // Convert the block array from string to int and store in stage_int
    void readCSV();

    // Function for reading config file, game settings and stuff
    void readConfig();
    
    // Getters
    std::vector<std::vector<uint8_t>> getStageInt();
};

#endif