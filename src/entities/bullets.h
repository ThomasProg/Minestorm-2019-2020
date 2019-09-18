#ifndef _BULLETS_H_
#define _BULLETS_H_

#include "vector2D/vector2D.h"
#include "SDL2/SDL.h"

typedef struct s_bullet
{
    float timeAlive;
    vector2D velocity;
    circle collision;

    bool isAlive;
} t_bullet;

t_bullet* bullet_create();
void bullet_init(t_bullet* bullet, vector2D location, vector2D relativeVelocity, vector2D shootDirection);
void bullet_destroy(t_bullet* bullet);

void bullet_render(SDL_Renderer* renderer, t_bullet* bullet, unsigned int nbLines);

void bullet_tick(t_bullet* bullet, float deltaTime);

#endif