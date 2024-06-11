#include "scene.h"

// Setters
void Scene::setDeltaTime(float dt) { delta_time = dt; }

// Getters
float Scene::getDeltaTime() { return delta_time; }

SDL_Window* Scene::getWindow() { return window; }