#ifndef _WORLD_H_
#define _WORLD_H_

#include "SDL_utilities/SDL_utilities.h"
#include "dynamicArray/dynamicArray.h"

#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"
#include "entities/player.h"

#include "game.h"
#include "level.h"

typedef struct 
{
    t_dynamicArray players;
    t_dynamicArray spawners;

    t_dynamicArray floatingMines;
    t_dynamicArray magneticMines;
    t_dynamicArray fireballMines;

    bool isPaused;
} t_world;

t_world* world_create();
void world_loop(t_assets* assets, float deltaTime, game* game);
void world_destroy(t_world* world);

void newWorldLevel(t_world* world, unsigned int level);

#endif