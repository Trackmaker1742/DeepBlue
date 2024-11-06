#include "object2d.h"

Object2d::Object2d(float X, float Y, const char* P) : 
    x(X), y(Y), path(P)
{ }

// Setters
void Object2d::setX(float X) { x = X; }
void Object2d::setY(float Y) { y = Y; }
void Object2d::setVelX(float vx) { vel_x = vx; }
void Object2d::setVelY(float vy) { vel_y = vy; }
void Object2d::setAccelX(float ax) { accel_x = ax; }
void Object2d::setAccelY(float ay) { accel_y = ay; }

// Getters
float Object2d::getX() { return x; }
float Object2d::getY() { return y; }
float Object2d::getVelX() { return vel_x; }
float Object2d::getVelY() { return vel_y; }
float Object2d::getAccelX() { return accel_x; }
float Object2d::getAccelY() { return accel_y; }
uint16_t Object2d::getGridX() { return x / default_grid; }
uint16_t Object2d::getGridY() { return y / default_grid; }

void Object2d::setGrid(uint16_t g) { grid = g; }
uint16_t Object2d::getGrid() { return grid; }

SDL_Texture *Object2d::getTexture() { return texture; }

// Initialization
void Object2d::initTexture(SDL_Renderer *renderer)
{
    // Empty path check
    if (path == "") return;
    
    // Load sprite into texture
    texture = IMG_LoadTexture(renderer, path);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Object2d::initTexture(const char *p, SDL_Renderer *renderer)
{
    // Empty path check
    if (p == "") 
    {
        return;
        std::cout << "no_sheet" << "\n";
    }
    // Load sprite into texture
    texture = IMG_LoadTexture(renderer, p);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

Object2d::~Object2d()
{
    // delete path;
    if (texture)
    {
        SDL_DestroyTexture(texture); // Clean up SDL texture
        texture = nullptr;
    }
}