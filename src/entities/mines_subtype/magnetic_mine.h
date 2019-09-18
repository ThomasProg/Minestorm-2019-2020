#ifndef _MAGNETIC_MINE_H_
#define _MAGNETIC_MINE_H_

//parent
#include "entities/mines.h"
#include "entities/player.h"

typedef struct s_magneticMine
{
    t_entity entity;
    t_player* target;

} t_magneticMine;

t_magneticMine* magneticMine_create();

void magneticMine_render(t_magneticMine* magneticMine, t_render* render);

void magneticMine_tick(t_magneticMine* magneticMine, float deltaTime);

#endif