#include "stage.h"

Stage::Stage(SDL_Renderer *rend) : renderer(rend)
{ }

// Getters
SDL_Texture *Stage::getBackground() { return bg; }
std::vector<SDL_Texture*> Stage::getBackgroundLayers() { return background_layers; }
std::vector<Block*> Stage::getBlockVec() { return Blocks; }
std::vector<Projectile*> Stage::getProjVec() { return Projectiles; }
uint16_t Stage::getRespX() { return resp_x; }
uint16_t Stage::getRespY() { return resp_y; }

// Setters
void Stage::setRespX(uint16_t x) { resp_x = x; }
void Stage::setRespY(uint16_t y) { resp_y = y; }

void Stage::initBackground()
{
    SDL_Surface* surface = IMG_Load("res/Menus/neon.png");
    bg = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Stage::initBlocks(const char *path)
{
    // Read stage layout straight from csv file
    File_Handler *file = new File_Handler();

    file->readCSV(path);

    for (int i = 0; i < file->getStageInt().size(); i++)
    {
        for (int j = 0; j < file->getStageInt()[i].size(); j++)
        {
            // Add blocks, skip 0 (empty space)
            // Numeric data for block (coming soon)

            int temp = file->getStageInt()[file->getStageInt().size() - 1 - i][j];

            // std::cout << temp << " " << block_path[temp - 1] << "\n";

            if (temp != 0) 
                Blocks.push_back(new Block(j, i, block_path[temp], temp));
        }
    }

    delete file;

    for (Block *b : Blocks)
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

void Stage::initRhyObs(const char *path)
{
    // Read stage layout straight from csv file
    File_Handler *file = new File_Handler();

    file->readCSV(path);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < file->getStageInt()[i].size(); j++)
        {
            // Add blocks, skip 0 (empty space)
            // Numeric data for block (coming soon)

            int temp = file->getStageInt()[file->getStageInt().size() - 1 - i][j];

            // std::cout << temp << " " << block_path[temp - 1] << "\n";

            if (temp != 0) 
            {
                if (temp == 2) Blocks.push_back(new Block(j, i + 1 + 1, temp, 3 - i, obstacle_path[1]));
                Blocks.push_back(new Block(j, i + 1, temp, 3 - i, obstacle_path[temp]));
            }   
        }
    }

    delete file;

    for (Block *b : Blocks)
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

void Stage::initPlatAll(const char *path)
{
    initBackground();
    initBlocks(path);
}

void Stage::initRhyAll(const char *path)
{
    initBackground();
    initRhyObs(path);
}

void Stage::unloadStage()
{
    // Clear all blocks from memory
    Blocks.clear();
    // Set spawn back to 0
    resp_x = 0;
    resp_y = 0;
}

Stage::~Stage()
{
    Blocks.clear();
    block_path.clear();
    obstacle_path.clear();
}