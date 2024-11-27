#ifndef STAGE_H
#define STAGE_H

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string.h>
#include <sstream>
#include <algorithm>

#include "config.h"
#include "file_handler.h"
#include "block.h"
#include "enemy.h"
#include "projectile.h"

class Stage
{
private:
    SDL_Renderer *renderer; // Not belonging to Stage

    static const int layer_count_max = 4;

    std::string layer_files[layer_count_max] = {"", "", "", ""};
    std::string layer_dirs[layer_count_max] = {"", "", "", ""};

    // Stage directory
    std::string stage_dir = "";
    
    // Block textures (for stage edit mode)
    std::vector<SDL_Texture*> block_textures;
    // Block names (for identifying correct block type + corresponding sprite)
    std::vector<std::string> block_names;
    // Block sprite path (able to detect the block type in csv)
    std::vector<std::string> block_paths;

    // Type index
    // 0: moving block
    // 1: normal block
    // 2: front block
    // 3: back block
    std::vector<std::vector<Block*>> blocks;

    // Background textures
    std::vector<SDL_Texture*> background_layers;
    
    std::vector<std::vector<std::string>> block_str;

    // Projectile array
    std::vector<Projectile*> projectiles;

    // Respawn coordinates
    uint16_t resp_x;
    uint16_t resp_y;

    // Background paths
    std::vector<std::string> background_paths;
    std::vector<std::vector<uint8_t>> background_parameter;
    uint8_t background_count_max = 0;

    // Rhythm obstacles sprites
    // High geyser sprite will only contain the bottom half,
    // with the top half using the same sprite as low geyser
    std::vector<std::string> obstacle_path;

    float scale_factor;

public:
    Stage();
    Stage(SDL_Renderer *rend);

    void init();

    // Getters
    std::string getLayerDir(int i);
    std::vector<SDL_Texture*> getBackgroundLayers();
    std::vector<SDL_Texture*> getBlockTextures();
    std::vector<Block*> getBlockVec(int i);
    std::vector<Projectile*> getProjVec();
    uint16_t getRespX();
    uint16_t getRespY();
    uint8_t getBgParam(int i, int j);
    uint8_t getBgCountMax();
    float getScaleFactor();

    std::string getPrefix(const std::string &str);
    std::string getSuffix(const std::string &str);

    // Setters
    void setRespX(uint16_t x);
    void setRespY(uint16_t y);

    void updateScaleFactor(float sf);
    void updateScale();
    // Initialize
    void initSpritePath(File_Handler *file, char stage_number);
    void initBackground();
    void initBlockEditTexture();

    void initMovingLayer(File_Handler *file, char stage_number);
    void initStaticLayer(File_Handler *file, char stage_number, uint8_t array_index);
    
    void initPlatAll(File_Handler *file, char stage_number);

    void initVertShooterAll(File_Handler *file, char stage_number);
    void initHoriShooterAll(File_Handler *file, char stage_number);

    void initRhyObs(File_Handler *file, char stage_number);
    void initRhyAll(File_Handler *file, char stage_number);

    // Update for moving blocks
    void update(float dt);

    // Block functions (moving, static)
    // Type index
    // 0: moving block
    // 1: normal block
    // 2: front block
    // 3: back block
    void addBlock(int x, int y, int index, uint8_t type);
    void deleteBlock(int index, uint8_t type);

    void clearTextureArray(std::vector<SDL_Texture*> textures);
    void clearBlockArray(std::vector<Block*> block_array);

    // Unload everything in a stage (blocks, spawn)
    void unload();

    void showState();

    ~Stage();
};

#endif