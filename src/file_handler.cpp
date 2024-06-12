#include "file_handler.h"

void File_Handler::readConfig()
{
    // std::cout << "Hello" << "\n";

    std::ifstream config;

    config.open(config_path);

    // Error checking
    if (!config.is_open())
    {
        std::cout << "File not opened" << "\n";
    }
    
    std::string temp;
    
    while (std::getline(config, temp, '|'))
    {
        config_array.push_back(temp);
        std::cout << temp << "\n";
    }

    // std::cout << "Hello 2" << "\n";
}