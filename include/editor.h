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
    bool saved = false;

    uint16_t grid = 0;

    uint16_t se_menu_counter = 0;

    uint16_t overlapped_block_index = 0;

    uint16_t x_max = 0;
    uint16_t y_max = 0;
    
    // Temp block array for faster iteration
    std::vector<Block*> temp_blocks;
    std::string stage_dir;

    bool is_block = false;
    uint16_t is_block_index = 0;

public:
    Editor();

    void setChanged(bool c);
    void setSaved(bool s);

    uint16_t getGrid();
    uint16_t getSEMCount();
    bool getChanged();
    bool getSaving();
    bool getSaved();

    void menuAction(Input *input, Player *player, Stage *stage);
    void saveChanges();
    bool playerBlockOverlap(Player *player, Stage *stage);
    void updateBlockArray(Input *input, std::vector<Block> &block_array);

    ~Editor();
};

#endif