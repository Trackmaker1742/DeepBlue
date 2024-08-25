#include "file_handler.h"

bool File_Handler::compareString(std::string line, std::string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if(tolower(line[i]) != tolower(str[i]))
        {
            return false;
        }
    }
    return true;
}

void File_Handler::removeChar(std::string& str, char ch)
{
    std::string temp;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != ch)
        {
            temp.push_back(str[i]);
        }
    }
    str = temp;
}

void File_Handler::grabConfigValue(std::string& row)
{
    // Remove all spaces
    removeChar(row, ' ');

    // Check type of data
    for (int i = 0; i < data_type.size(); i++)
    {
        // Skip to next loop if the data type don't match
        if (!compareString(row, data_type[i])) continue;
        else 
        {
            // Take value and append to a string
            std::string value_str = "";
            for (int j = data_type[i].length() + 1; j < row.length(); j++)
            {
                value_str.push_back(row[j]);
            }
            // Then convert to int and save the value to a variable
            values[i] = std::stoi(value_str);
        }
    }
}

void File_Handler::grabSaveValue(std::string& row)
{
    // Remove all spaces
    removeChar(row, ' ');

    // Check type of data
    // If stage data
    if (compareString(row, save_data_type[0]))
    {
        // Take value and append to a string
        std::string value_str = "";
        for (int i = save_data_type[0].length() + 1; i < row.length(); i++)
        {
            value_str.push_back(row[i]);
        }
        // Then convert to int and save the value to a variable
        save_values[0] = std::stoi(value_str);
    }
    // If item data
    else if (compareString(row, save_data_type[1]))
    {
        // Input data will be similar to this: 010
        // 01: Item number 1
        // 0: Collection status

        // Take value and append to a string
        std::string value_str = "";
        for (int i = save_data_type[1].length() + 1; i < row.length(); i++)
        {
            value_str.push_back(row[i]);
        }
        // Identify the current item
        if (value_str[0] == '0') save_values[int(value_str[1])] = int(value_str[3]);
        else
        {
            std::string temp = "";
            temp = temp + value_str[0] + value_str[1];
            save_values[std::stoi(temp)] = int(value_str[3]);
        }
    }
}

void File_Handler::readConfig()
{
    // Open config
    std::ifstream config(config_path);

    // Error checking
    if (!config.is_open())
    {
        std::cout << "Unable to open config" << "\n";
    }
    
    // Parsing relevant data
    std::string temp_row;
    while (std::getline(config, temp_row))
    {
        // Skip empty or comment row
        if (temp_row.empty() || temp_row[0] == '#') continue;
        
        // Grab value from the row
        grabConfigValue(temp_row);
    }
    
    // Debug
    // std::cout << values[0] << " " << values[1] << "\n";
    
    // Close config
    config.close();
}

void File_Handler::readSave()
{
    // Open save file
    std::ifstream save(save_path);
    
    // Error checking
    if (!save.is_open())
    {
        std::cout << "Unable to open save file" << "\n";
    }
    
    // Parsing relevant data
    std::string temp_row;
    while (std::getline(save, temp_row))
    {
        // Skip empty or comment row
        if (temp_row.empty() || temp_row[0] == '#') continue;
        
        // Grab value from the row
        grabSaveValue(temp_row);
    }

    // Close file 
    save.close();
}

void File_Handler::readCSV(char stage_number, 
    std::vector<std::vector<std::string>> &blocks_str)
{
    std::string path = "res/Stages/Stage ";
    path += stage_number;
    path += "/block_layer.csv";

    // Open stage layout csv
    std::ifstream layout(path);

    // Parsing relevant data
    std::string temp_row_str;
    while(std::getline(layout, temp_row_str))
    {
        std::stringstream row_stream(temp_row_str);
        std::string value_str;
        std::vector<std::string> temp_row_array;
        // Process each value separated by ','
        while (std::getline(row_stream, value_str, ',')) 
        {
            temp_row_array.push_back(value_str);
        }
        blocks_str.push_back(temp_row_array);
    }

    // Debug
    // std::cout << stage_int.size() << " ";
    // std::cout << stage_int[1].size() << " ";
    // std::cout << stage_int[2].size() << " ";
        
    // for (int i = 0; i < stage_int.size(); i++)
    // {
    //     for (int j = 0; j < stage_int[i].size(); j++)
    //     {
    //         std::cout << stage_int[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // Close file
    layout.close();
}

void File_Handler::readAssetFolders(char stage_number, 
    std::vector<std::string> &bg_layers, 
    std::vector<std::string> &blocktiles,
    bool &bg_move)
{
    std::string path = "res/Stages/Stage ";
    path += stage_number;
    std::string file_name = "";
    std::string temp = "";
    
    // Iterate through background folder
    temp = path + "/background/";
    for (const auto& entry : std::filesystem::directory_iterator(temp)) 
    {
        // If the entry is a regular file, add its name to the vector
        if (std::filesystem::is_regular_file(entry.status())) {
            if (entry.path().filename().string() == "move")
            {
                bg_move = true;
                continue;
            }
            file_name = temp + entry.path().filename().string();
            bg_layers.push_back(file_name);
        }
    }

    temp = "";

    // Iterate through blocktile folder
    temp = path + "/blocktile/";
    for (const auto& entry : std::filesystem::directory_iterator(temp)) {
        // If the entry is a regular file, add its name to the vector
        if (std::filesystem::is_regular_file(entry.status())) {
            file_name = temp + entry.path().filename().string();
            blocktiles.push_back(file_name);
        }
    }
}

uint16_t File_Handler::getValue(int i) { return values[i]; }

File_Handler::~File_Handler()
{
    csv_paths.clear();
}