#include "stage.h"

Stage::Stage(SDL_Renderer *rend) : renderer(rend)
{ }

// Getters
std::vector<SDL_Texture*> Stage::getBackgroundLayers() { return background_layers; }
std::vector<Block*> Stage::getBlockVec() { return blocks; }
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
    for (std::string path : background_paths)
    {
        SDL_Surface* surface = IMG_Load(path.c_str());
        background_layers.push_back(SDL_CreateTextureFromSurface(renderer, surface));
        SDL_FreeSurface(surface);
    }
    background_paths.clear();
}

void Stage::initBlockLayer(char stage_number)
{
    // Read stage layout straight from csv file
    File_Handler *file = new File_Handler();

    file->readCSV(stage_number, block_int);

    for (int i = 0; i < block_int.size(); i++)
    {
        for (int j = 0; j < block_int[i].size(); j++)
        {
            // Add blocks, skip 0 (empty space)
            // Numeric data for block (coming soon)

            int temp = block_int[block_int.size() - 1 - i][j];

            // std::cout << temp << " " << block_path[temp - 1] << "\n";

            if (temp != 0) 
                blocks.push_back(new Block(j, i, block_paths[temp - 1].c_str(), temp));
        }
    }

    // Clear int array, delete file object
    block_int.clear();
    delete file;

    // Texture and spawn handler
    for (Block *b : blocks)
    {
        b->initTexture(renderer);

        // Set spawn point
        if (b->getType() == 1)
        {
            resp_x = b->getGridX();
            resp_y = b->getGridY();
        }
    }
}

void Stage::initFrontLayer(char stage_number)
{
    
}

void Stage::initRhyObs(char stage_number)
{
    // Read stage layout straight from csv file
    File_Handler *file = new File_Handler();

    // Empty all arrays before initializing
    block_int.clear();
    blocks.clear();

    // Read and store in integer array
    file->readCSV(stage_number, block_int);

    // Convert integer array into block array
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < block_int[i].size(); j++)
        {
            // Add blocks, skip 0 (empty space)
            // Numeric data for block (coming soon)

            int temp = block_int[block_int.size() - 1 - i][j];

            // std::cout << temp << " " << block_path[temp - 1] << "\n";

            if (temp != 0) 
            {
                if (temp == 2) blocks.push_back(new Block(j, i + 1 + 1, temp, 3 - i, obstacle_path[1].c_str()));
                blocks.push_back(new Block(j, i + 1, temp, 3 - i, obstacle_path[temp].c_str()));
            }   
        }
    }

    // Clear int array, delete file object
    block_int.clear();
    delete file;

    // Texture and spawn handler
    for (Block *b : blocks)
    {
        b->initTexture(renderer);

        // Set spawn point
        if (b->getType() == 1)
        {
            resp_x = b->getGridX();
            resp_y = b->getGridY();
        }
    }
}

void Stage::initPlatAll(char stage_number)
{
    initSpritePath(stage_number);
    initBackground();
    // Curently stuck at block layer
    initBlockLayer(stage_number);
}

void Stage::initRhyAll(char stage_number)
{
    initBackground();
    initRhyObs(stage_number);
}

void Stage::unloadStage()
{
    // Clear all blocks from memory
    blocks.clear();
    background_paths.clear();
    background_layers.clear();
    block_int.clear();
    block_paths.clear();
    obstacle_path.clear();
    // Set spawn back to 0
    resp_x = 0;
    resp_y = 0;
}

Stage::~Stage()
{
    blocks.clear();
    background_paths.clear();
    background_layers.clear();
    block_int.clear();
    block_paths.clear();
    obstacle_path.clear();
}