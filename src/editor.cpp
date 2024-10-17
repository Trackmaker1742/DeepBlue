#include "editor.h"

Editor::Editor()
{ }

void Editor::setChanged(bool c) { changed = c; };

uint16_t Editor::getGrid() { return grid; }
uint16_t Editor::getSEMCount() { return se_menu_counter; }
bool Editor::getChanged() { return changed; }
bool Editor::getSaved() { return saved; }

void Editor::menuAction(Input *input, Player *player, Stage *stage)
{
    grid = player->getGrid() / 2;
    // Return to normal mode
    if (input->getPress(Action::EXTRA1) && !changed)
    {
        input->setHold(Action::EXTRA1, false);
        player->setEditor(false);
    }
    // Menu navigation
    if (input->getPress(Action::EXTRA_UP) &&
        se_menu_counter > 5)
    {
        input->setHold(Action::EXTRA_UP, false);
        se_menu_counter -= 6;
    }
    if (input->getPress(Action::EXTRA_DOWN) &&
        se_menu_counter < stage->getBlockTextures().size() - 1)
    {
        input->setHold(Action::EXTRA_DOWN, false);
        se_menu_counter += 6;
    }
    if (input->getPress(Action::EXTRA_LEFT) &&
        se_menu_counter > 0)
    {
        input->setHold(Action::EXTRA_LEFT, false);
        se_menu_counter--;
    }
    if (input->getPress(Action::EXTRA_RIGHT) &&
        se_menu_counter < stage->getBlockTextures().size() - 1)
    {
        input->setHold(Action::EXTRA_RIGHT, false);
        se_menu_counter++;
    }
    // Adding block
    if (input->getPress(Action::ACTION1) && !playerBlockOverlap(player, stage))
    {
        input->setHold(Action::ACTION1, false);
        stage->addBlock(
            int(player->getCenterX() / grid), 
            int(player->getCenterY() / grid),
            se_menu_counter
        );
        changed = true;
        saved = false;
    }
    // Removing block
    if (input->getPress(Action::ACTION2) && playerBlockOverlap(player, stage))
    {
        input->setHold(Action::ACTION2, false);
        stage->deleteBlock(overlapped_block_index);
        changed = true;
        saved = false;
    }
    if (input->getPress(Action::EXTRA2) && !saved && changed)
    {
        input->setHold(Action::EXTRA2, false);
        saved = true;
    }
    if (changed && saved)
    {
        temp_blocks = stage->getBlockVec();
        stage_dir = stage->getStageDir();
        stage_dir += "block_layer.csv";
    }
    // Note to self, work on the rendering, moving block creation, 
    // just general stuff that would make level creation not a pain for anyone that's not me
}
void Editor::saveChanges()
{
    // Save function
    for (Block *b : temp_blocks)
    {
        // Initializing the max value of x and y for writing into file
        if (x_max == 0) x_max = b->getGridX();
        if (y_max == 0) y_max = b->getGridY();
        // Assign max value
        if (x_max < b->getGridX()) x_max = b->getGridX();
        if (y_max < b->getGridY()) y_max = b->getGridY();
    }
    std::cout << x_max << " " << y_max << "\n";
    // Create and write into file
    std::ofstream b_layer;
    // Open and clear file
    b_layer.open(stage_dir, std::ios::out | std::ios::trunc);
    // Iterate and write into file
    for (int i = y_max; i >= 0; --i)
    {
        for (int j = 0; j < x_max; j++)
        {
            for (int k = 0; k < temp_blocks.size(); k++)
            {
                if (temp_blocks[k]->getGridX() == j && 
                    temp_blocks[k]->getGridY() == i)
                {
                    is_block = true;
                    is_block_index = k;  
                    break; 
                }
                else is_block = false;
            }
            if (is_block)
            {
                // std::cout << "is_block\n";
                // Adding prefix for block with type name of less than 2 digits
                if (temp_blocks[is_block_index]->getType() < 10) b_layer << "0";
                // Write block type
                b_layer << int(temp_blocks[is_block_index]->getType()) << ",";
                // Delete block from temp array to help save computation
                // delete temp_blocks[is_block_index];
                // temp_blocks[is_block_index] = nullptr;
                // temp_blocks.erase(temp_blocks.begin() + is_block_index);
            }
            else b_layer << "0,";
        }
        b_layer << "\n";
    }
    b_layer.close();
}

bool Editor::playerBlockOverlap(Player *player, Stage *stage)
{
    for (int i = 0; i < stage->getBlockVec().size(); i++)
    {
        if (player->getCenterX() > stage->getBlockVec()[i]->getX() &&
        player->getCenterX() < stage->getBlockVec()[i]->getX() + stage->getBlockVec()[i]->getGrid() &&
        player->getCenterY() > stage->getBlockVec()[i]->getY() &&
        player->getCenterY() < stage->getBlockVec()[i]->getY() + stage->getBlockVec()[i]->getGrid())
        {
            // std::cout << stage->getBlockVec()[i]->getGridX() << " " << stage->getBlockVec()[i]->getGridY() << "\n";
            overlapped_block_index = i;
            return true;
        }
    }
    return false;
}

Editor::~Editor()
{
    std::cout << "Editor terminated!\n";
}