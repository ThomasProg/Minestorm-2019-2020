#ifndef _BULLETS_H_
#define _BULLETS_H_

#include "vector2D/vector2D.h"
#include "SDL2/SDL.h"

typedef struct s_bullet
{
    float timeAlive;
    vector2D velocity;
    circle collision;

    bool isBullet; //else, is fireball
    unsigned int playerOwnerID;
} t_bullet;

void bullet_init(t_bullet* bullet, vector2D location, vector2D relativeVelocity, 
                 vector2D shootDirection, bool isBullet);

void bullet_render(SDL_Renderer* renderer, t_bullet* bullet, unsigned int nbLines);

void bullet_tick(t_bullet* bullet, float deltaTime);

#endif