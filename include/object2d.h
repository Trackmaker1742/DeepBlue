#ifndef OBJECT2D_H
#define OBJECT2D_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "scene.h"

class Object2d
{
private:
    // Position
    float x;
    float y;

    // Size
    uint16_t width;
    uint16_t height;

    // Velocity
    float vel_x = 0;
    float vel_y = 0;

    // Acceleration
    float accel_x = 0;
    float accel_y = 0;
        
    // Grid size
    uint16_t grid = 64;
    
    // Sprite handler
    const char* path;
    SDL_Surface* surface;
    SDL_Texture* texture;

public:
    Object2d(float X = 0, float Y = 0, const char* P = "");

    // Setters
    void setX(float x);
    void setY(float y);
    void setVelX(float vx);
    void setVelY(float vy);
    void setAccelX(float ax);
    void setAccelY(float ay);

    // Getters
    float getX();
    float getY();
    float getVelX();
    float getVelY();
    float getAccelX();
    float getAccelY();
    uint16_t getGridX();
    uint16_t getGridY();

    void setGrid(uint16_t g);
    uint16_t getGrid();

    SDL_Texture* getTexture();

    // Intitializations
    void initTexture(SDL_Renderer* renderer);
    void initTexture(const char *p, SDL_Renderer *renderer);

    ~Object2d();
};

#endif