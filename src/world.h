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
    //arrays of poiners
    //t_entity** entities;
    t_player** players;
    vector2D* spawners;
    unsigned int nbSpawners;

    
    //t_fireballMine** fireballsMines;
    t_dynamicArray floatingMines;
    t_dynamicArray magneticMines;
    t_dynamicArray fireballMines;
    //t_floatingMine** floatingMines;
    //unsigned int nbfloatingMines;
    //t_magneticMine** magneticMines;

} t_world;

t_world* world_create();
void world_loop(t_assets* assets, float deltaTime, game* game);
void world_destroy(t_world* world);

void newWorldLevel(t_world* world, unsigned int level);

#endif