#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>
#include <fstream>
#include <vector>

#include "input.h"
#include "player.h"
#include "stage.h"
#include "block.h"

class Editor
{
private:
    static const int max_block_type = 4;

    bool changed[max_block_type] = {false, false, false, false};
    
    // bool changed = false;
    bool saving = false;
    bool saved = true;
    bool reset = true;

    // Type index
    // 0: moving block
    // 1: normal block
    // 2: front block
    // 3: back block
    uint8_t block_type_counter = 1;

    bool initial_m_block = true;

    int initial_m_block_x;
    int initial_m_block_y;
    uint16_t initial_m_block_se_menu_counter;

    uint16_t grid = 0;

    uint16_t se_menu_counter = 0;

    uint16_t overlapped_block_index = 0;

    uint16_t x_max = 0;
    uint16_t y_max = 0;

    bool is_block = false;
    uint16_t block_index = 0;

public:
    Editor();

    void setSaving(bool s);
    void setSaved(bool s);

    uint16_t getGrid();
    uint16_t getSEMCount();
    bool getChanged(int i);
    bool getSaving();
    bool getSaved();
    uint8_t getBlockTypeCounter();
    bool getInitMBlock();

    void init();
    void completeSaving();

    void menuAction(Input *input, Player *player, Stage *stage);
    
    void setMaxXYSize(std::vector<Block*> blocks);
    void saveMovingBlocks(std::vector<Block*> blocks, std::string layer_dir);
    void saveNormalBlocks(std::vector<Block*> blocks, std::string layer_dir);
    void saveNoColliBlocks(std::vector<Block*> blocks, std::string layer_dir, int type);
    bool playerBlockOverlap(Player *player, std::vector<Block*> blocks);
    // void updateBlockArray(Input *input, std::vector<Block> &block_array);

    ~Editor();
};

#endif