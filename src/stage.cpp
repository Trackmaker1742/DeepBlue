#include "stage.h"

Stage::Stage()
{ }

Stage::Stage(SDL_Renderer *rend) : renderer(rend)
{ }

// Getters
std::string Stage::getStageDir() { return stage_dir; }
std::vector<SDL_Texture*> Stage::getBackgroundLayers() { return background_layers; }
std::vector<SDL_Texture*> Stage::getBlockTextures() { return block_textures; }
std::vector<Block*> Stage::getBlockVec() { return blocks; }
std::vector<Block*> Stage::getMovingBlockVec() { return moving_blocks; }
std::vector<Projectile*> Stage::getProjVec() { return projectiles; }
uint16_t Stage::getRespX() { return resp_x; }
uint16_t Stage::getRespY() { return resp_y; }
uint8_t Stage::getBgParam(int i, int j) { return background_parameter[i][j]; }
uint8_t Stage::getBgCountMax() { return background_count_max; }

// Setters
void Stage::setRespX(uint16_t x) { resp_x = x; }
void Stage::setRespY(uint16_t y) { resp_y = y; }

void Stage::initSpritePath(File_Handler *file, char stage_number)
{
    // Saving stage directory for editor mode
    stage_dir = "res/Stages/Stage ";
    stage_dir += stage_number;
    stage_dir += '/';

    file->readAllStageAssets(stage_number, 
        background_paths,
        background_parameter,
        block_names,
        block_paths);
    // Set maximum background number
    background_count_max = background_parameter.size();

    // for (std::string path : block_paths)
    // {
    //     std::cout << path << "\n";
    // }

    std::cout << "Sprite Paths Initialized!\n";
}
void Stage::initBackground()
{
    int i = 0;
    for (std::string path : background_paths)
    {
        background_layers.push_back(IMG_LoadTexture(renderer, path.c_str()));
        path.erase(path.begin() + i);
        i++;
    }
    background_paths.clear();

    std::cout << "Background Initialized!\n";
}
void Stage::initBlockEditTexture()
{
    for (std::string path : block_paths)
    {
        block_textures.push_back(IMG_LoadTexture(renderer, path.c_str()));
    }

    std::cout << "Editor Textures Initialized!\n";
}

// For analyzing block names
std::string Stage::getPrefix(const std::string &str) 
{
    // Find the position of the first dash
    size_t pos = str.find('-');
    // Return the substring before the dash, or the whole string if no dash is found
    return (pos != std::string::npos) ? str.substr(0, pos) : str;
}
std::string Stage::getSuffix(const std::string &str)
{
    // Find the position of the first dash
    size_t pos = str.find('-');
    // Return the substring after the dash
    return str.substr(pos + 1, str.length());
}

void Stage::initBlockLayer(File_Handler *file, char stage_number)
{
    // Read stage layout straight from csv file
    file->readCSV(stage_number, block_str);
    // for (auto bs : block_str)
    // {
    //     for (auto b : bs)
    //         std::cout << b << ",";
    //     std::cout << "\n";
    // }

    std::string cur_block;
    int temp_asset_index;

    // Convert normal block string array into block array
    // Gonna have to overhaul a lot of this
    for (int i = 0; i < block_str.size(); i++)
    {
        for (int j = 0; j < block_str[i].size(); j++)
        {
            cur_block = block_str[block_str.size() - 1 - i][j];
            // Skip if there is no block
            if (cur_block == "0") continue;
            else
            {
                // Add blocks
                // Works with both blocks with 1 sprite and blocks with multiple sprites
                if (cur_block.length() < 6)
                {
                    // Parsing blocks name
                    for (int k = 0; k < block_names.size(); k++)
                    {
                        // If the block in the array matches with the name of the asset,
                        // save the index of the block to use for block init
                        if (cur_block == block_names[k])
                        {
                            temp_asset_index = k;
                            break;
                        }
                    }
                    blocks.push_back(new Block
                    (
                        j, 
                        i, 
                        block_paths[temp_asset_index].c_str(), 
                        std::stoi(getPrefix(cur_block))
                    ));
                    // Sprite indexing for block with multiple sprites
                    if (cur_block.length() > 2)
                    {
                        blocks.back()->setHasSpriteIndex(true);
                        blocks.back()->setSpriteIndex(std::stoi(getSuffix(cur_block)));
                    }
                    else blocks.back()->setHasSpriteIndex(false);
                }
                // Adding moving blocks to block array
                else
                {
                    // A counter is used to keep track of which field of data is currently being saved
                    // In 06|10|11|01, 06 would have a counter of 0, 01 would have a counter of 3
                    std::stringstream data_stream(cur_block);
                    std::string value_str;
                    int counter = 0;
                    while(std::getline(data_stream, value_str, '|'))
                    {
                        switch (counter)
                        {
                            // Block type
                            case 0:
                                // Parsing blocks name
                                for (int k = 0; k < block_names.size(); k++)
                                {
                                    if (value_str == block_names[k])
                                    {
                                        temp_asset_index = k;
                                        break;
                                    }
                                }
                                moving_blocks.push_back(new Block
                                (
                                    j,
                                    i,
                                    block_paths[temp_asset_index].c_str(), 
                                    std::stoi(getPrefix(value_str))
                                ));
                                // Sprite indexing for block with multiple sprites
                                if (value_str.length() > 2)
                                {
                                    moving_blocks.back()->setHasSpriteIndex(true);
                                    moving_blocks.back()->setSpriteIndex(std::stoi(getSuffix(value_str)));
                                }
                                else moving_blocks.back()->setHasSpriteIndex(false);
                                counter++;
                                break;
                            // Travel dist x
                            case 1:
                                moving_blocks.back()->setTravelDistX(std::stoi(value_str) * moving_blocks.back()->getGrid());
                                counter++;
                                break;
                            // Travel dist y
                            case 2:
                                moving_blocks.back()->setTravelDistY(std::stoi(value_str) * moving_blocks.back()->getGrid());
                                counter++;
                                break;
                            // Movement type and init
                            case 3:
                                moving_blocks.back()->initMove(std::stoi(value_str));
                                counter = 0;
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
    // Init block texture and spawn
    for (Block *b : blocks)
    {
        b->initTexture(renderer);

        // Set spawn point
        if (b->getType() == 1)
        {
            resp_x = b->getX();
            resp_y = b->getY();
        }
    }
    // Init moving block texture
    for (Block *b : moving_blocks)
    {
        b->initTexture(renderer);
    }

    std::cout << "Block Layer Initialized!\n";
}

void Stage::initFrontLayer(File_Handler *file, char stage_number)
{
    
}

// void Stage::initRhyObs(File_Handler *file, char stage_number)
// {
//     // Clear block arrays
//     block_str.clear();
//     moving_blocks_str.clear();
    
//     front_str.clear();

//     blocks.clear();
//     moving_blocks.clear();
//     front_blocks.clear();

//     // Read stage layout straight from csv file
//     File_Handler *file = new File_Handler();

//     // Read and store in integer array
//     // file->readCSV(stage_number, block_int);

//     // Convert integer array into block array
//     for (int i = 0; i < 3; i++)
//     {
//         for (int j = 0; j < block_int[i].size(); j++)
//         {
//             // Add blocks, skip 0 (empty space)
//             // Numeric data for block (coming soon)

//             int temp = block_int[block_int.size() - 1 - i][j];

//             // std::cout << temp << " " << block_path[temp - 1] << "\n";

//             if (temp != 0) 
//             {
//                 if (temp == 2) blocks.push_back(new Block(j, i + 1 + 1, temp, 3 - i, obstacle_path[1].c_str()));
//                 blocks.push_back(new Block(j, i + 1, temp, 3 - i, obstacle_path[temp].c_str()));
//             }   
//         }
//     }

//     // Clear int array
//     block_int.clear();

//     // Texture and spawn handler
//     for (Block *b : blocks)
//     {
//         b->initTexture(renderer);

//         // Set spawn point
//         if (b->getType() == 1)
//         {
//             resp_x = b->getGridX();
//             resp_y = b->getGridY();
//         }
//     }
// }

void Stage::initPlatAll(File_Handler *file, char stage_number)
{
    initSpritePath(file, stage_number);
    initBackground();
    initBlockEditTexture();
    initBlockLayer(file, stage_number);
    std::cout << "Platformer Stage Initialized!\n";
}

void Stage::initVertShooterAll(File_Handler *file, char stage_number)
{
    initSpritePath(file, stage_number);
    initBackground();
    // Init enemy array or something, idk
}

void Stage::initHoriShooterAll(File_Handler *file, char stage_number)
{
    initSpritePath(file, stage_number);
    initBackground();
    // Init enemy array or something, idk
}

// void Stage::initRhyAll(char stage_number)
// {
//     initBackground();
//     initRhyObs(stage_number);
// }

void Stage::update(float dt)
{
    // Moving block start & end array should have the same size, 
    // so there won't be any issue reading both using 1 iterator
    // Start at 1 because index formatting
    for (Block *b: moving_blocks)
    {
        b->move(dt);
    }
}

// void Stage::stageEditAction(Input *input, float x, float y, uint16_t grid)
// {
//     // Menu navigation
//     if (input->getPress(Action::EXTRA_UP) &&
//         se_menu_counter > 5)
//     {
//         input->setHold(Action::EXTRA_UP, false);
//         se_menu_counter -= 6;
//     }
//     if (input->getPress(Action::EXTRA_DOWN) &&
//         se_menu_counter < block_textures.size() - 1)
//     {
//         input->setHold(Action::EXTRA_DOWN, false);
//         se_menu_counter += 6;
//     }
//     if (input->getPress(Action::EXTRA_LEFT) &&
//         se_menu_counter > 0)
//     {
//         input->setHold(Action::EXTRA_LEFT, false);
//         se_menu_counter--;
//     }
//     if (input->getPress(Action::EXTRA_RIGHT) &&
//         se_menu_counter < block_textures.size() - 1)
//     {
//         input->setHold(Action::EXTRA_RIGHT, false);
//         se_menu_counter++;
//     }
//     // Adding block
//     if (input->getPress(Action::ACTION1))
//     {
//         input->setHold(Action::ACTION1, false);
//         blocks.push_back(new Block(int(x/grid), int(y/grid), block_paths[se_menu_counter].c_str(), std::stoi(getPrefix(block_names[se_menu_counter]))));
//         blocks.back()->initTexture(renderer);
//     }
// }

void Stage::addBlock(int x, int y, int index, bool move)
{
    if (!move)
    {
        blocks.push_back(new Block(
            x, 
            y, 
            block_paths[index].c_str(), 
            std::stoi(getPrefix(block_names[index]))));
        if (block_names[index].length() > 2)
        {
            blocks.back()->setHasSpriteIndex(true);
            blocks.back()->setSpriteIndex(std::stoi(getSuffix(block_names[index])));
        }
        blocks.back()->initTexture(renderer);
        std::cout << block_names[index] << " block added\n";
    }
    else
    {
        moving_blocks.push_back(new Block(
            x, 
            y, 
            block_paths[index].c_str(), 
            std::stoi(getPrefix(block_names[index]))));
        if (block_names[index].length() > 2)
        {
            moving_blocks.back()->setHasSpriteIndex(true);
            moving_blocks.back()->setSpriteIndex(std::stoi(getSuffix(block_names[index])));
        }
        moving_blocks.back()->initTexture(renderer);
        moving_blocks.back()->initMove(false);
        std::cout << "moving block added\n";
    }
}
void Stage::deleteBlock(int index, bool move)
{
    if (!move)
    {
        delete blocks[index];
        blocks[index] = nullptr;
        blocks.erase(blocks.begin() + index);
        std::cout << "block deleted\n";
    }
    else
    {
        delete moving_blocks[index];
        moving_blocks[index] = nullptr;
        moving_blocks.erase(moving_blocks.begin() + index);
        std::cout << "moving block deleted\n";
    }
}

void Stage::unload()
{
    // Pointer vectors, need to be deleted
    for (SDL_Texture* texture : block_textures) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);    // Free the texture resource
            texture = nullptr;              // Optional: Set to nullptr for safety
        }
    }
    block_textures.clear();

    block_names.clear();
    block_paths.clear();

    for (int i = 0; i < blocks.size(); i++)
    {
        delete blocks[i];
        blocks[i] = nullptr;
    }
    blocks.clear();
    for (int i = 0; i < moving_blocks.size(); i++)
    {
        delete moving_blocks[i];
        moving_blocks[i] = nullptr;
    }
    moving_blocks.clear();

    for (SDL_Texture* texture : background_layers) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
    background_layers.clear();

    block_str.clear();
    front_str.clear();

    background_paths.clear();
    background_parameter.clear();
    
    // Set spawn back to 0
    resp_x = 0;
    resp_y = 0;
}

Stage::~Stage()
{
    // blocks.clear();
    // background_paths.clear();
    // background_layers.clear();
    // block_paths.clear();
    // obstacle_path.clear();
    std::cout << "Stage terminated!\n";
}