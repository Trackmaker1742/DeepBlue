#include "file_handler.h"

void File_Handler::readConfig()
{
    std::ifstream config(config_path);

    // Error checking
    if (config.is_open())
    {
        std::cout << "File is opened" << "\n";
    }
    
    std::string temp;
    
    while (std::getline(config, temp, '|'))
    {
        // config_array.push_back(temp);
        std::cout << temp << "\n";
    }
}