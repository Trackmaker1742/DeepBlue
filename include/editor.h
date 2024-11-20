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
    // If true, player can't exit edit mode until either changes are saved or discarded
    bool changed = false;
    bool saving = false;
    bool saved = true;
    bool reset = true;

    // Type index
    // 0: normal block
    // 1: moving block
    // 2: front block
    uint8_t block_type_counter = 0;

    bool initial_m_block = true;

    int initial_m_block_x;
    int initial_m_block_y;
    uint16_t initial_m_block_se_menu_counter;

    uint16_t grid = 0;

    uint16_t se_menu_counter = 0;

    uint16_t overlapped_block_index = 0;

    uint16_t x_max = 0;
    uint16_t y_max = 0;
    
    // Temp block array for faster iteration
    std::string block_layer_dir;
    std::string front_layer_dir;

    bool is_block = false;
    uint16_t block_index = 0;
    bool is_moving_block = false;
    uint16_t moving_block_index = 0;

public:
    Editor();

    void setChanged(bool c);
    void setSaved(bool s);

    uint16_t getGrid();
    uint16_t getSEMCount();
    bool getChanged();
    bool getSaving();
    bool getSaved();
    uint8_t getBlockTypeCounter();
    bool getInitMBlock();

    void menuAction(Input *input, Player *player, Stage *stage);
    void saveChanges(std::vector<Block*> blocks, 
        std::vector<Block*> m_blocks,
        std::vector<Block*> f_blocks);
    bool playerBlockOverlap(Player *player, std::vector<Block*> blocks);
    // void updateBlockArray(Input *input, std::vector<Block> &block_array);

    ~Editor();
};

#endif