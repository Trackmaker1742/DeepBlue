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

void File_Handler::removeChar(std::string str, char ch)
{
    std::string temp;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != ch)
        {
            temp.push_back(str[i]);
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
        std::cout << "Config not opened" << "\n";
    }
    
    std::string temp;
    
    // Save all strings without comments into array
    while (std::getline(config, temp))
    {
        if (temp.empty() || temp[0] == '#') continue;
        // Data filtering, removing all separator "|"
        config_array.push_back(temp);
    }

    // Seek for the line that contains resolution data
    for (std::string test : config_array)
    {
        if (compareString(test, "resolution")) reso_str = test;
        break;
    }

    // Extracting resolution data
    removeChar(reso_str, ' ');
    // Template: resolution|{&screen_width}|{&screen_height}
    uint16_t screen_width = 0;
    uint16_t screen_height = 0;
    bool count = 0;
    std::string temp1 = "";
    for (int i = 11; i < reso_str.length(); i++)
    {
        if (reso_str[i] != '|') 
        {
            temp1 += reso_str[i];
            std::cout << temp1;
        }
        else
        {
            // if (!count)
            //     screen_width = std::stoi(temp1);
            // else
            //     screen_height = std::stoi(temp1);
            temp1 = "";
            std::cout << "delim" << "\n";
        }
    }

    // std::cout << screen_width << " " << screen_height << "\n";
    
    // Stored data output
    // for (std::string test : config_array)
    // {
    //     std::cout << screen_width << " " << screen_height << "\n";
    // }

    // Close config
    config.close();
}