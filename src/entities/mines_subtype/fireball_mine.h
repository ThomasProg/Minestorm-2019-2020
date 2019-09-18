#ifndef _FIREBALL_MINE_H_
#define _FIREBALL_MINE_H_

#include "entities/mines.h"

typedef struct s_fireballMine
{
    t_entity entity;

} t_fireballMine;

t_fireballMine* fireballMine_create();

void fireballMine_render(t_fireballMine* fireballMine, t_render* render);

void fireballMine_tick(t_fireballMine* fireballMine, float deltaTime);

#endif