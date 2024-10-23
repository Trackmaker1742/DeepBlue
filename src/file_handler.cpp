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
    // std::cout << values[0] << " " << values[1] << "\n";
    
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

void File_Handler::readStageAssetFolders(char stage_number, 
    std::vector<std::string> &bg_layers, 
    std::vector<std::string> &block_names,
    std::vector<std::string> &block_paths,
    std::vector<std::vector<uint8_t>> &bg_param)
{
    std::string path = "res/Stages/Stage ";
    path += stage_number;
    std::string file_path = "";
    std::string temp = "";
    
    // Iterate through background folder
    // Parallax fix zone
    temp = path + "/background/";
    for (const auto& entry : std::filesystem::directory_iterator(temp)) 
    {
        // If the entry is a regular file, add its name to the vector
        if (std::filesystem::is_regular_file(entry.status())) {
            // Do stuff when encountering the param file, skip to the next file
            if (entry.path().filename().string() == "param.txt")
            {
                // Open param file
                file_path = temp + entry.path().filename().string();
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
            file_path = temp + entry.path().filename().string();
            bg_layers.push_back(file_path);
        }
    }

    temp = "";

    // Iterate through blocktile folder
    temp = "res/Blocktile/";
    for (const auto& entry : std::filesystem::directory_iterator(temp)) {
        // If the entry is a regular file, add its name to the vector
        if (std::filesystem::is_regular_file(entry.status())) {
            file_path = entry.path().filename().string();
            // Trimming the .png portion of the string
            // Saving block name into array for stage design and indexing
            block_names.push_back(file_path.substr(0, file_path.length() - 4));
            // Adding directory to the file
            file_path = temp + file_path;
            // Saving entire block path into an array
            block_paths.push_back(file_path); 
        }
    }

    // Iterate through blocktile folder
    temp = temp + "Stage " + stage_number + '/';
    for (const auto& entry : std::filesystem::directory_iterator(temp)) {
        // If the entry is a regular file, add its name to the vector
        if (std::filesystem::is_regular_file(entry.status())) {
            file_path = entry.path().filename().string();
            // Trimming the .png portion of the string
            // Saving block name into array for stage design and indexing
            block_names.push_back(file_path.substr(0, file_path.length() - 4));
            // Adding directory to the file
            file_path = temp + file_path;
            // Saving entire block path into an array
            block_paths.push_back(file_path);
        }
    }
}

uint16_t File_Handler::getValue(int i) { return values[i]; }

File_Handler::~File_Handler()
{
    // csv_paths.clear();
}