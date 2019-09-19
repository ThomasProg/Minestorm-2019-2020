#ifndef _MINES_H_
#define _MINES_H_

#include <entities/entities.h>

// typedef struct s_player
// {
    
//     t_entity entity;

// } t_mines;

void mine_collisionBox_init(polygon* collision, 
    unsigned int nbPolygons, float size1, float size2);

void mine_spawn(void* mine, unsigned int type, vector2D* spawners, unsigned int nbSpawners);
#endif