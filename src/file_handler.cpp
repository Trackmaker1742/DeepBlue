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

void File_Handler::grabConfigValue(std::string &row)
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

void File_Handler::grabSaveValue(std::string &row)
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

void File_Handler::grabBgParamValue(std::string &row)
{
    // Remove all spaces
    removeChar(row, ' ');

    // Check type of data
    for (int i = 0; i < bg_param_data_type.size(); i++)
    {
        // Skip to next loop if the data type don't match
        if (!compareString(row, bg_param_data_type[i])) continue;
        else 
        {
            // Take value and append to a string
            std::string value_str = "";
            for (int j = bg_param_data_type[i].length() + 1; j < row.length(); j++)
            {
                value_str.push_back(row[j]);
            }
            // If encounter an index 
            // Save index value to assign the correct value to each background img
            if (compareString(row, bg_param_data_type[0])) 
            {
                current_bg_index = std::stoi(value_str) - 1;
                bg_param_values.push_back({0,0});
                // std::cout << current_bg_index << "\n";
            }
            // Assign auto-scroll speed value (0 means bg doesn't scroll automatically)
            if (compareString(row, bg_param_data_type[1])) 
                bg_param_values[current_bg_index][0] = std::stoi(value_str);
            // Assign speed value
            if (compareString(row, bg_param_data_type[2])) 
                bg_param_values[current_bg_index][1] = std::stoi(value_str);
        }
    }
}

void File_Handler::readConfig()
{
    // Open config
    std::ifstream config(config_path);
    
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
    // std::cout << values[0] << " " << values[1] << " " << values[2] << " " << values[3] << "\n";
    
    // Close config
    config.close();
}

void File_Handler::readSave()
{
    // Open save file
    std::ifstream save(save_path);
    
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
    std::string layer_file,
    std::vector<std::vector<std::string>> &blocks_str)
{
    // Open stage layout csv
    std::ifstream layout(stage_dir + stage_number + layer_file);

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

    // Close file
    layout.close();
}

int File_Handler::gameModeCheck(std::string stage_directory)
{
    // Define a map of files and their corresponding return values
    std::unordered_map<std::string, int> file_checks = 
    {
        {"platformer", 1},
        {"shooter", 2},
        {"rhythm", 3}
        // Add more files here as needed
    };

    for (const auto& [file_name, return_value] : file_checks) 
    {
        if (std::filesystem::exists(stage_directory + file_name)) 
        {
            return return_value;
        }
    }

    // Return a default value if no file is found
    return 0;
}

void File_Handler::readStageBgAsset(std::string directory, 
    std::vector<std::string> &bg_layers,
    std::vector<std::vector<uint8_t>> &bg_param)
{
    file_path = "";

    // Iterate through stage background assets
    for (const auto& entry : std::filesystem::directory_iterator(directory)) 
    {
        // If the entry is a regular file, add its name to the vector
        if (std::filesystem::is_regular_file(entry.status())) {
            // Do stuff when encountering the param file, skip to the next file
            if (entry.path().filename().string() == background_param_file)
            {
                // Open param file
                file_path = directory + entry.path().filename().string();
                std::ifstream param(file_path);

                // Parsing relevant data
                std::string temp_line;
                while (std::getline(param, temp_line))
                {
                    // Skip empty or comment row
                    if (temp_line.empty() || temp_line[0] == '#') continue;

                    // Grab value from the row
                    grabBgParamValue(temp_line);
                }

                // Close file 
                param.close();
            }
            for (std::vector<uint8_t> values : bg_param_values)
            {
                bg_param.push_back(values);
            }
            bg_param_values.clear();
            // Adding background files to the list
            file_path = directory + entry.path().filename().string();
            bg_layers.push_back(file_path);
        }
    }
}

void File_Handler::readBlockAsset(std::string directory,
    std::vector<std::string> &block_names,
    std::vector<std::string> &block_paths)
{
    file_path = "";

    // Iterate through shared blocks
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        // If the entry is a regular file, add its name to the vector
        if (std::filesystem::is_regular_file(entry.status())) {
            file_path = entry.path().filename().string();
            // Trimming the .png portion of the string
            // Saving block name into array for stage design and indexing
            block_names.push_back(file_path.substr(0, file_path.length() - 4));
            // Adding directory to the file
            file_path = directory + file_path;
            // Saving entire block path into an array
            block_paths.push_back(file_path); 
        }
    }
}

void File_Handler::readAllStageAssets(char stage_number, 
    std::vector<std::string> &bg_layers,
    std::vector<std::vector<uint8_t>> &bg_param, 
    std::vector<std::string> &block_names,
    std::vector<std::string> &block_paths)
{
    // Iterate through background folder
    readStageBgAsset(stage_dir + stage_number + background_folder, bg_layers, bg_param);

    // Skip reading block assets for shooter stages
    if (gameModeCheck(stage_dir + stage_number + '/') == 2) return;

    // Iterate through general blocktile folder
    readBlockAsset(block_dir, block_names, block_paths);
    // Iterate through stage specific blocktile folder
    readBlockAsset(stage_dir + stage_number + stage_block_folder, block_names, block_paths);
}

void File_Handler::readMenuBgAsset(
    std::vector<std::vector<std::string>> &bg_paths)
{
    file_path = "";
    folder_path = "";
    
    for (const auto &folder : std::filesystem::directory_iterator(menu_dir)) {
        // If the entry is a directory (which it will be)
        if (std::filesystem::is_directory(folder.status()))
        {
            folder_path = menu_dir + folder.path().filename().string();
            std::vector<std::string> temp_str_array;
            // Iterate all files
            for (const auto &file_entry : std::filesystem::directory_iterator(folder_path))
            {
                file_path = folder_path + '/' + file_entry.path().filename().string();
                temp_str_array.push_back(file_path);
            }
            bg_paths.push_back(temp_str_array);
        }
        else continue;
    }
}

void File_Handler::writeSave(std::string resolution, int display_option, std::string framerate)
{
    std::istringstream reso(resolution);

    char separator = 'x';

    reso >> values[0] >> separator >> values[1];

    values[2] = display_option;
    values[3] = std::stoi(framerate);

    std::ofstream config(config_path, std::ios::out | std::ios::trunc);

    config << "Width | " << values[0] << "\n"
        << "Height | " << values[1] << "\n"
        << "Display_option | " << values[2] << "\n"
        << "FPS | " << values[3] << "\n";

    config.close();
}

uint16_t File_Handler::getValue(int i) { return values[i]; }

File_Handler::~File_Handler()
{
    std::cout << "File Handler terminated!\n";
}