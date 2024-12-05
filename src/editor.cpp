#include "editor.h"

Editor::Editor()
{ }

void Editor::setSaving(bool s) { saving = s; }
void Editor::setSaved(bool s) { saved = s; }

uint16_t Editor::getGrid() { return grid; }
uint16_t Editor::getSEMCount() { return se_menu_counter; }
bool Editor::getChanged(int i) { return changed[i]; }
bool Editor::getSaving() { return saving; }
bool Editor::getSaved() { return saved; }
uint8_t Editor::getBlockTypeCounter() { return block_type_counter; }
bool Editor::getInitMBlock() { return initial_m_block; }

void Editor::init()
{
    x_max = 0;
    y_max = 0;
}
void Editor::completeSaving() 
{
    saving = false;
    saved = true;
};

void Editor::menuAction(Input *input, Player *player, Stage *stage)
{
    grid = player->getGrid() / 2;
    if (reset)
    {
        for (Block *b : stage->getBlockVec(0))
        {
            b->reset();
        }
        initial_m_block = true;
        reset = false;
    }
    // Return to normal mode
    if (input->getPress(Action::EXTRA1) && initial_m_block && saved && !saving)
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
        // Moving block
        case 0:
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
                        0
                    );
                    stage->getBlockVec(0).back()->setTravelDistGridX
                    (
                        int(player->getCenterX() / grid) - stage->getBlockVec(0).back()->getX()
                    );
                    stage->getBlockVec(0).back()->setTravelDistGridY
                    (
                        int(player->getCenterY() / grid) - stage->getBlockVec(0).back()->getY()
                    );
                    stage->getBlockVec(0).back()->init(stage->getScaleFactor());
                    stage->getBlockVec(0).back()->initMove(false);
                    initial_m_block = true;
                }
                changed[block_type_counter] = true;
                saved = false;
            }
        break;
        // Normal block
        case 1: case 2:
            if (input->getHold(Action::ACTION1) &&
                !playerBlockOverlap(player, stage->getBlockVec(1)) &&
                !playerBlockOverlap(player, stage->getBlockVec(2)))
            {
                stage->addBlock
                (
                    int(player->getCenterX() / grid), 
                    int(player->getCenterY() / grid),
                    se_menu_counter,
                    block_type_counter
                );
                changed[block_type_counter] = true;
                saved = false;
            }
        break;
        // Normal block
        case 3:
            if (input->getHold(Action::ACTION1) &&
                !playerBlockOverlap(player, stage->getBlockVec(3)))
            {
                stage->addBlock
                (
                    int(player->getCenterX() / grid), 
                    int(player->getCenterY() / grid),
                    se_menu_counter,
                    block_type_counter
                );
                changed[block_type_counter] = true;
                saved = false;
            }
        break;
        default:
        break;
    }
    // Removing normal, moving block
    if (input->getHold(Action::ACTION2))
    {
        for (int i = 0; i < max_block_type; i++)
        {
            if (block_type_counter == i && 
                playerBlockOverlap(player, stage->getBlockVec(i)))
            {
                stage->deleteBlock(overlapped_block_index, i);
                changed[i] = true;
                saved = false;
            }
        }
    }
    
    // Save button
    if (input->getPress(Action::EXTRA2) && !saved && !saving && initial_m_block)
    {
        saving = true;
    }
    // Switch to adding moving block
    if (input->getPress(Action::EXTRA3))
    {
        block_type_counter++;
        block_type_counter %= max_block_type;
        if (!saved && !saving && initial_m_block) saving = true;
    }
}

void Editor::setMaxXYSize(std::vector<Block*> blocks)
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
}

void Editor::saveMovingBlocks(std::vector<Block*> blocks, std::string layer_dir)
{
    // Create and write into moving_layer.csv
    std::ofstream m_layer;
    // Open and clear file
    m_layer.open(layer_dir, std::ios::out | std::ios::trunc);
    // Iterate and write into file
    for (int i = y_max; i >= 0; i--)
    {
        for (int j = 0; j <= x_max; j++)
        {
            // Moving blocks
            for (int k = 0; k < blocks.size(); k++)
            {
                if (blocks[k]->getInitGridX() == j && 
                    blocks[k]->getInitGridY() == i)
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
                if (blocks[block_index]->getType() < 10) m_layer << "0";
                // Write block type
                m_layer << int(blocks[block_index]->getType());
                // Write sprite index for block with a different sprite than the default one
                if (blocks[block_index]->getHasSpriteIndex()) 
                {
                    m_layer << '-';
                    // Adding prefix
                    if (blocks[block_index]->getSpriteIndex() < 10) m_layer << "0";
                    m_layer << int(blocks[block_index]->getSpriteIndex());
                }
                m_layer << "|"
                // Write distance x
                << int(blocks[block_index]->getTravelDistGridX()) << "|"
                // Write distance y
                << int(blocks[block_index]->getTravelDistGridY()) << "|"
                // Write type (manual or auto)
                << blocks[block_index]->getManual() << ",";
                continue;
            }
            else m_layer << "0,";
        }
        m_layer << "\n";
    }
    m_layer.close();

    changed[0] = false;
}
void Editor::saveNormalBlocks(std::vector<Block*> blocks, std::string layer_dir)
{
    // Create and write into block_layer.csv
    std::ofstream n_layer;
    // Open and clear file
    n_layer.open(layer_dir, std::ios::out | std::ios::trunc);
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
                if (blocks[block_index]->getType() < 10) n_layer << "0";
                // Write block type
                n_layer << int(blocks[block_index]->getType());
                // Write sprite index for block with a different sprite than the default one
                if (blocks[block_index]->getHasSpriteIndex()) 
                {
                    n_layer << '-';
                    // Adding prefix
                    if (blocks[block_index]->getSpriteIndex() < 10) n_layer << "0";
                    n_layer << int(blocks[block_index]->getSpriteIndex());
                }
                n_layer << ",";
                continue;   // Very important, if this isn't here, extra moving blocks will be added, leading to malloc error
            }
            else n_layer << "0,";
        }
        n_layer << "\n";
    }
    n_layer.close();
    
    changed[1] = false;
}
void Editor::saveNoColliBlocks(std::vector<Block*> blocks, std::string layer_dir, int type)
{
    // Create and write into front_layer.csv
    std::ofstream nc_layer;
    // Open and clear file
    nc_layer.open(layer_dir, std::ios::out | std::ios::trunc);
    // Iterate and write into file
    for (int i = y_max; i >= 0; i--)
    {
        for (int j = 0; j <= x_max; j++)
        {
            // Normal f_blocks
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
                nc_layer << blocks[block_index]->getBlockName() << ",";
                continue;   // Very important, if this isn't here, extra moving f_blocks will be added, leading to malloc error
            }
            else nc_layer << "0,";
        }
        nc_layer << "\n";
    }
    nc_layer.close();
    
    changed[type] = false;
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