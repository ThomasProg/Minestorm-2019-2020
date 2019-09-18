#ifndef _WORLD_H_
#define _WORLD_H_

#include "SDL_utilities/SDL_utilities.h"
#include "entities/entities.h"
#include "entities/player.h"

typedef struct 
{
    //arrays of poiners
    //t_entity** entities;
    t_player** players;
} t_world;

t_world* world_create();
void world_loop(t_assets* assets, float deltaTime, t_world* world);
void world_destroy(t_world* world);

#endif