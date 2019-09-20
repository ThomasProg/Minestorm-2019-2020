#ifndef _MINES_H_
#define _MINES_H_

#include <entities/entities.h>
#include "entities/player.h"

// typedef struct s_player
// {
    
//     t_entity entity;

// } t_mines;

// typedef enum
// {
//     FLOATING_MINE = 0,
//     MAGNETIC_MINE,
//     FIREBALL_MINE,
//     MAGNETIC_FIREBALL_MINE
// } E_MINE;

typedef struct s_mine
{
    bool followPlayer;
    bool throwFireballs;

    float size; 
    
    t_entity entity;
    t_player* target;

} t_mine;

void mine_init(t_mine* mine, unsigned int type, vector2D location, float size);
void mine_destroy(t_mine* mine);

void mine_render(t_mine* mine, t_render* render);

void mine_tick(t_mine* mine, float deltaTime);

void mine_collisionBox_init(polygon* collision, 
    unsigned int nbPolygons, float size1, float size2);

void mine_spawn(void* mine, unsigned int type, vector2D spawner);

#endif