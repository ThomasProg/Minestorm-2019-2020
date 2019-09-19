#ifndef _FLOATING_MINE_H_
#define _FLOATING_MINE_H_

#include "entities/mines.h"

typedef struct s_floatingMine
{
    
    t_entity entity;

} t_floatingMine;

t_floatingMine* floatingMine_create(); //unused
void floatingMine_init(t_floatingMine* floatingMine);

void floatingMine_render(t_floatingMine* floatingMine, t_render* render);

void floatingMine_tick(t_floatingMine* floatingMine, float deltaTime);

#endif