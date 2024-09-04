#include "stage.h"

Stage::Stage(SDL_Renderer *rend) : renderer(rend)
{ }

// Getters
std::vector<SDL_Texture*> Stage::getBackgroundLayers() { return background_layers; }
std::vector<Block*> Stage::getBlockVec() { return blocks; }
std::vector<Block*> Stage::getMovingBlockVec() { return moving_blocks; }
std::vector<Projectile*> Stage::getProjVec() { return projectiles; }
uint16_t Stage::getRespX() { return resp_x; }
uint16_t Stage::getRespY() { return resp_y; }
bool Stage::getBgMove() { return background_move; }

// Setters
void Stage::setRespX(uint16_t x) { resp_x = x; }
void Stage::setRespY(uint16_t y) { resp_y = y; }

void Stage::initSpritePath(char stage_number)
{
    File_Handler *file = new File_Handler();
    file->readAssetFolders(stage_number, 
        background_paths, 
        block_paths,
        background_move);

    // for (std::string path : block_paths)
    // {
    //     std::cout << path << "\n";
    // }

    delete file;
}

void Stage::initBackground()
{
    int i = 0;
    for (std::string path : background_paths)
    {
        SDL_Surface* surface = IMG_Load(path.c_str());
        background_layers.push_back(SDL_CreateTextureFromSurface(renderer, surface));
        SDL_FreeSurface(surface);
        path.erase(path.begin() + i);
        i++;
    }
    background_paths.clear();
}

void Stage::initBlockLayer(char stage_number)
{
    // Clear block arrays
    block_str.clear();
    front_str.clear();

    // Pointer vectors, need to be deleted
    // blocks.push_back(new Block(1, 1, "", 4));
    // for (int i = 0; i < blocks.size(); i++)
    // {
    //     std::cout << "delete blocks" << "\n";
    //     delete blocks[i];
    //     blocks.erase(blocks.begin() + i);
    // }
    blocks.clear();
    moving_blocks.clear();
    front_blocks.clear();

    // Read stage layout straight from csv file
    File_Handler *file = new File_Handler();

    file->readCSV(stage_number, block_str);

    // Convert normal block string array into block array
    for (int i = 0; i < block_str.size(); i++)
    {
        for (int j = 0; j < block_str[i].size(); j++)
        {
            // Add blocks, skip 0 (empty space)
            // Numeric data for block (coming soon)
            // Adding normal blocks to block array
            if (block_str[block_str.size() - 1 - i][j].length() < 3)
            {
                int temp = std::stoi(block_str[block_str.size() - 1 - i][j]);
                if (temp != 0) 
                    blocks.push_back(new Block(j, i, block_paths[temp - 1].c_str(), temp));
            }
            // Adding moving blocks to block array
            else
            {
                // A counter is used to keep track of which field of data is currently being saved
                // In 06|10|11|01, 06 would have a counter of 0, 01 would have a counter of 3
                std::stringstream data_stream(block_str[block_str.size() - 1 - i][j]);
                std::string value_str;
                int counter = 0;
                while(std::getline(data_stream, value_str, '|'))
                {
                    int temp1;
                    switch (counter)
                    {
                        // Block type
                        case 0:
                            temp1 = std::stoi(value_str);
                            moving_blocks.push_back(new Block(j, i, block_paths[temp1 - 1].c_str(), temp1));
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

    // Delete file object
    delete file;
}

void Stage::initFrontLayer(char stage_number)
{
    
}

// void Stage::initRhyObs(char stage_number)
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

//     // Clear int array, delete file object
//     block_int.clear();
//     delete file;

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

void Stage::initPlatAll(char stage_number)
{
    initSpritePath(stage_number);
    initBackground();
    initBlockLayer(stage_number);
}

void Stage::initVertShooterAll(char stage_number)
{
    initSpritePath(stage_number);
    initBackground();
    // Init enemy array or something, idk
}

void Stage::initHoriShooterAll(char stage_number)
{
    initSpritePath(stage_number);
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

void Stage::unloadStage()
{
    background_paths.clear();
    block_paths.clear();
    obstacle_path.clear();

    // Pointer vectors, need to be deleted
    for (int i = 0; i < blocks.size(); i++)
    {
        delete blocks[i];
        blocks.erase(blocks.begin() + i);
    }
    blocks.clear();
    background_layers.clear();
    
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