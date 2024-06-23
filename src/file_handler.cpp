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

void File_Handler::readCSV(const char *path)
{
    // Open stage layout csv
    std::ifstream layout(path);

    // Error checking
    if (!layout.is_open())
    {
        std::cout << "Unable to open csv file" << "\n";
    }

    // Parsing relevant data
    std::string temp_row_str;
    while(std::getline(layout, temp_row_str))
    {
        // Take value and append to a string
        std::string value_str = "";
        std::vector<uint8_t> temp_row_int;
        for (int i = 0; i < temp_row_str.length(); i++)
        {
            if (temp_row_str[i] != ',')
            {
                // std::cout << temp_row_str[i] << " ";
                value_str.push_back(temp_row_str[i]);
            }
            else
            {
                // Save int to stage layout array
                // std::cout << std::stoi(value_str) << " ";
                
                temp_row_int.push_back(std::stoi(value_str));
                value_str = "";
            }
        }
        stage_int.push_back(temp_row_int);
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

uint16_t File_Handler::getValue(int i) { return values[i]; }
std::vector<std::vector<uint8_t>> File_Handler::getStageInt() { return stage_int; }