#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

class File_Handler 
{
private:
    const char *config_path = "data/config.txt";
    // Data type string for parsing
    std::vector<std::string> data_type = 
    {
        "width",
        "height",
        "fps",
        "display option",
        "scaling"
    };
    // Array for values (same order as data type for easier parsing)
    std::vector<uint16_t> values =
    {
        0,
        0,
        0,
        0,
        0
    };

    const char *save_path = "data/save.txt";
    // Data type in save file
    std::vector<std::string> save_data_type =
    {
        "stage",
        "item"
    };
    // Data value in save file
    std::vector<uint8_t> save_values =
    {
        1,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0
    };
    
    // Data type in param file
    int current_bg_index = 0;
    std::vector<std::string> bg_param_data_type =
    {
        "index",
        "autoscroll-speed",
        "speed"
    };
    // Data value in save file
    std::vector<std::vector<uint8_t>> bg_param_values;

    std::vector<const char*> csv_paths;    

public:
    // Checking if a line has a specific string
    bool compareString(std::string line, std::string str);

    // Remove a certain character in a string
    void removeChar(std::string &str, char ch);

    // Grab value from a data row
    void grabConfigValue(std::string &row);

    // Grab save value from a row
    void grabSaveValue(std::string &row);

    // Grab param value from a row
    void grabBgParamValue(std::string &row);

    // Basically no diff between the 2, but I'm too lazy to fix it
    // So it'll stay, for now
    // Function for reading game settings, configs
    void readConfig();
    // Function for reading save file
    void readSave();
    
    // Function for reading stage layout in CSV form
    // and store in string, because moving block needs extra processing
    void readCSV(char stage_number, 
        std::vector<std::vector<std::string>> &blocks_str);

    // Parse a full stage folder
    // The idea is to combine all the read and store function to create a sort of template
    // to make levels more easily
    // Blocktile will be in a separate folder
    // The folder structure will be:
    // Stage ?
    // |
    // |-background
    //  |-1-4.png
    // |-back_layer.csv
    // |-block_layer.csv
    // |-front_layer.csv
    void readStageAssetFolders(char stage_number, 
        std::vector<std::string> &bg_layers,
        std::vector<std::string> &block_names,
        std::vector<std::string> &block_paths,
        std::vector<std::vector<uint8_t>> &bg_param);

    // Getters
    uint16_t getValue(int i);

    ~File_Handler();
};

#endif