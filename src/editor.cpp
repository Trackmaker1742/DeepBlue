#include "editor.h"

Editor::Editor()
{ }

void Editor::setChanged(bool c) { changed = c; };
void Editor::setSaved(bool s) { saved = s; }

uint16_t Editor::getGrid() { return grid; }
uint16_t Editor::getSEMCount() { return se_menu_counter; }
bool Editor::getChanged() { return changed; }
bool Editor::getSaving() { return saving; }
bool Editor::getSaved() { return saved; }

void Editor::menuAction(Input *input, Player *player, Stage *stage)
{
    grid = player->getGrid() / 2;
    // Return to normal mode
    if (input->getPress(Action::EXTRA1) && !changed && !saving)
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
    if (input->getPress(Action::EXTRA2) && !saved && changed && !saving)
    {
        input->setHold(Action::EXTRA2, false);
        saving = true;
    }
    if (changed && saving)
    {
        stage_dir = stage->getStageDir();
        stage_dir += "block_layer.csv";
    }
    // Note to self, work on the rendering, moving block creation, 
    // just general stuff that would make level creation not a pain for anyone that's not me
}
void Editor::saveChanges(std::vector<Block*> blocks, std::vector<Block*> m_blocks)
{
    // Save function
    for (Block *b : blocks)
    {
        // Initializing the max value of x and y for writing into file
        if (x_max == 0) x_max = b->getGridX();
        if (y_max == 0) y_max = b->getGridY();
        // Assign max value
        if (x_max < b->getGridX()) x_max = b->getGridX();
        if (y_max < b->getGridY()) y_max = b->getGridY();
    }
    for (Block *b : m_blocks)
    {
        // Assign max value
        if (x_max < b->getInitGridX()) x_max = b->getInitGridX();
        if (y_max < b->getInitGridY()) y_max = b->getInitGridY();
    }
    // Create and write into file
    std::ofstream b_layer;
    // Open and clear file
    b_layer.open(stage_dir, std::ios::out | std::ios::trunc);
    // Iterate and write into file
    for (int i = y_max; i >= 0; i--)
    {
        for (int j = 0; j < x_max; j++)
        {
            // Normal blocks
            for (int k = 0; k < blocks.size(); k++)
            {
                if (blocks[k]->getGridX() == j && 
                    blocks[k]->getGridY() == i)
                {
                    is_block = true;
                    block_index = k;  
                    break; 
                }
                else is_block = false;
            }
            if (is_block)
            {
                // Adding prefix for block with type name of less than 2 digits
                if (blocks[block_index]->getType() < 10) b_layer << "0";
                // Write block type
                b_layer << int(blocks[block_index]->getType()) << ",";
            }
            // Moving blocks
            else
            {
                for (int k = 0; k < m_blocks.size(); k++)
                {
                    if (m_blocks[k]->getInitGridX() == j && 
                        m_blocks[k]->getInitGridY() == i)
                    {
                        is_moving_block = true;
                        moving_block_index = k;  
                        break; 
                    }
                    else is_moving_block = false;
                }
            }
            if (is_moving_block)
            {
                // Adding prefix for block with type name of less than 2 digits
                if (m_blocks[moving_block_index]->getType() < 10) b_layer << "0";
                // Write block type
                b_layer << int(m_blocks[moving_block_index]->getType()) << "|"
                // Write distance x
                << int(m_blocks[moving_block_index]->getTravelDistGridX()) << "|"
                // Write distance y
                << int(m_blocks[moving_block_index]->getTravelDistGridY()) << "|"
                // Write type (manual or auto)
                << m_blocks[moving_block_index]->getManual() << ",";
            }
            else if (!is_block && !is_moving_block) b_layer << "0,";
        }
        b_layer << "\n";
    }
    b_layer.close();

    saving = false;
    saved = true;

    std::cout << "New Layout Saved!\n";
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