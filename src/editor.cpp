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
uint8_t Editor::getBlockTypeCounter() { return block_type_counter; }
bool Editor::getInitMBlock() { return initial_m_block; }

void Editor::menuAction(Input *input, Player *player, Stage *stage)
{
    grid = player->getGrid() / 2;
    if (reset)
    {
        for (Block *b : stage->getMovingBlockVec())
        {
            b->reset();
        }
        initial_m_block = true;
        reset = false;
    }
    // Return to normal mode
    if (input->getPress(Action::EXTRA1) && !changed && !saving && initial_m_block)
    {
        // input->setHold(Action::EXTRA1, false);
        player->setEditor(false);
        reset = true;
        return;
    }
    // Menu navigation
    if (input->getPress(Action::EXTRA_UP) &&
        se_menu_counter > 5)
    {
        // input->setHold(Action::EXTRA_UP, false);
        se_menu_counter -= 6;
    }
    if (input->getPress(Action::EXTRA_DOWN) &&
        se_menu_counter < stage->getBlockTextures().size() - 1)
    {
        // input->setHold(Action::EXTRA_DOWN, false);
        se_menu_counter += 6;
    }
    if (input->getPress(Action::EXTRA_LEFT) &&
        se_menu_counter > 0)
    {
        // input->setHold(Action::EXTRA_LEFT, false);
        se_menu_counter--;
    }
    if (input->getPress(Action::EXTRA_RIGHT) &&
        se_menu_counter < stage->getBlockTextures().size() - 1)
    {
        // input->setHold(Action::EXTRA_RIGHT, false);
        se_menu_counter++;
    }
    // Adding, removing block
    switch (block_type_counter)
    {
        // Normal block
        case 0:
            if (input->getHold(Action::ACTION1) && 
                !playerBlockOverlap(player, stage->getBlockVec()) &&
                !playerBlockOverlap(player, stage->getMovingBlockVec()) &&
                !playerBlockOverlap(player, stage->getFrontVec()))
            {
                stage->addBlock
                (
                    int(player->getCenterX() / grid), 
                    int(player->getCenterY() / grid),
                    se_menu_counter
                );
                changed = true;
                saved = false;
                std::cout << "block added\n";
            }
        break;
        // Moving block
        case 1:
            if (input->getPress(Action::ACTION1))
            {
                if (initial_m_block)
                {
                    initial_m_block_x = player->getCenterX() / grid;
                    initial_m_block_y = player->getCenterY() / grid;
                    initial_m_block_se_menu_counter = se_menu_counter;
                    initial_m_block = false;
                }
                else
                {
                    stage->addBlock
                    (
                        initial_m_block_x,
                        initial_m_block_y,
                        initial_m_block_se_menu_counter,
                        1
                    );
                    stage->getMovingBlockVec().back()->setTravelDistGridX
                    (
                        int(player->getCenterX() / grid) - stage->getMovingBlockVec().back()->getX()
                    );
                    stage->getMovingBlockVec().back()->setTravelDistGridY
                    (
                        int(player->getCenterY() / grid) - stage->getMovingBlockVec().back()->getY()
                    );
                    stage->getMovingBlockVec().back()->init(stage->getScaleFactor());
                    stage->getMovingBlockVec().back()->initMove(false);
                    initial_m_block = true;
                }
                changed = true;
                saved = false;
                std::cout << "moving block added\n";
            }
        break;
        // Front block
        case 2:
            if (input->getHold(Action::ACTION1) && 
                !playerBlockOverlap(player, stage->getBlockVec()) && 
                !playerBlockOverlap(player, stage->getFrontVec()))
            {
                stage->addBlock
                (
                    int(player->getCenterX() / grid), 
                    int(player->getCenterY() / grid),
                    se_menu_counter,
                    2
                );
                changed = true;
                saved = false;
                std::cout << "front block added\n";
            }
        break;
        default:
        break;
    }
    // Removing normal, moving block
    if (input->getHold(Action::ACTION2))
    {
        // If cursor overlaps normal block
        if (playerBlockOverlap(player, stage->getBlockVec()))
        {
            stage->deleteBlock(overlapped_block_index);
            changed = true;
            saved = false;
        }
        // If cursor overlaps moving block
        if (playerBlockOverlap(player, stage->getMovingBlockVec()))
        {
            stage->deleteBlock(overlapped_block_index, 1);
            changed = true;
            saved = false;
        }
        // If cursor overlaps moving block
        if (playerBlockOverlap(player, stage->getFrontVec()))
        {
            stage->deleteBlock(overlapped_block_index, 2);
            changed = true;
            saved = false;
        }
    }
    
    // Save button
    if (input->getPress(Action::EXTRA2) && !saved && changed && !saving)
    {
        // input->setHold(Action::EXTRA2, false);
        if (initial_m_block) saving = true;
    }
    // Switch to adding moving block
    if (input->getPress(Action::EXTRA3))
    {
        if (block_type_counter < 2) block_type_counter++;
        else block_type_counter = 0;
    }
    
    if (changed && saving)
    {
        block_layer_dir = stage->getStageDir();
        block_layer_dir += "block_layer.csv";
        
        front_layer_dir = stage->getStageDir();
        front_layer_dir += "front_layer.csv";
    }
}
void Editor::saveChanges(std::vector<Block*> blocks, 
    std::vector<Block*> m_blocks,
    std::vector<Block*> f_blocks)
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

    // Create and write into block_layer.csv
    std::ofstream b_layer;
    // Open and clear file
    b_layer.open(block_layer_dir, std::ios::out | std::ios::trunc);
    // Iterate and write into file
    for (int i = y_max; i >= 0; i--)
    {
        for (int j = 0; j <= x_max; j++)
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
                b_layer << int(blocks[block_index]->getType());
                // Write sprite index for block with a different sprite than the default one
                if (blocks[block_index]->getHasSpriteIndex()) 
                {
                    b_layer << '-';
                    // Adding prefix
                    if (blocks[block_index]->getSpriteIndex() < 10) b_layer << "0";
                    b_layer << int(blocks[block_index]->getSpriteIndex());
                }
                b_layer << ",";
                continue;   // Very important, if this isn't here, extra moving blocks will be added, leading to malloc error
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
                b_layer << int(m_blocks[moving_block_index]->getType());
                // Write sprite index for block with a different sprite than the default one
                if (m_blocks[moving_block_index]->getHasSpriteIndex()) 
                {
                    b_layer << '-';
                    // Adding prefix
                    if (m_blocks[moving_block_index]->getSpriteIndex() < 10) b_layer << "0";
                    b_layer << int(m_blocks[moving_block_index]->getSpriteIndex());
                }
                b_layer << "|"
                // Write distance x
                << int(m_blocks[moving_block_index]->getTravelDistGridX()) << "|"
                // Write distance y
                << int(m_blocks[moving_block_index]->getTravelDistGridY()) << "|"
                // Write type (manual or auto)
                << m_blocks[moving_block_index]->getManual() << ",";
                continue;
            }
            else if (!is_block && !is_moving_block) b_layer << "0,";
        }
        b_layer << "\n";
    }
    b_layer.close();

    // Create and write into front_layer.csv
    std::ofstream f_layer;
    // Open and clear file
    f_layer.open(front_layer_dir, std::ios::out | std::ios::trunc);
    // Iterate and write into file
    for (int i = y_max; i >= 0; i--)
    {
        for (int j = 0; j <= x_max; j++)
        {
            // Normal f_blocks
            for (int k = 0; k < f_blocks.size(); k++)
            {
                if (f_blocks[k]->getGridX() == j && 
                    f_blocks[k]->getGridY() == i)
                {
                    is_block = true;
                    block_index = k;  
                    break; 
                }
                else is_block = false;
            }
            if (is_block)
            {
                f_layer << f_blocks[block_index]->getBlockName() << ",";
                continue;   // Very important, if this isn't here, extra moving f_blocks will be added, leading to malloc error
            }
            else f_layer << "0,";
        }
        f_layer << "\n";
    }
    f_layer.close();

    saving = false;
    saved = true;

    std::cout << "New Layout Saved!\n";
}

bool Editor::playerBlockOverlap(Player *player, std::vector<Block*> blocks)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (player->getCenterX() > blocks[i]->getX() &&
        player->getCenterX() < blocks[i]->getX() + blocks[i]->getGrid() &&
        player->getCenterY() > blocks[i]->getY() &&
        player->getCenterY() < blocks[i]->getY() + blocks[i]->getGrid())
        {
            // std::cout << blocks[i]->getGridX() << " " << blocks[i]->getGridY() << "\n";
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