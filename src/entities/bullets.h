#ifndef _BULLETS_H_
#define _BULLETS_H_

#include "vector2D/vector2D.h"
#include "SDL2/SDL.h"

typedef struct s_bullet
{
    vector2D velocity;
    circle collision;
} t_bullet;

t_bullet* bullet_create(vector2D location, vector2D velocity);

void bullet_render(SDL_Renderer* renderer, t_bullet* bullet, unsigned int nbLines);

void bullet_tick(t_bullet* bullet, float deltaTime);

#endif