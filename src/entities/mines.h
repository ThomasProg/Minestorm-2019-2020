#ifndef _MINES_H_
#define _MINES_H_

#include <entities/entities.h>

// typedef struct s_player
// {
    
//     t_entity entity;

// } t_mines;

void mine_collisionBox_init(convexPolygonsArray* collision, 
    unsigned int nbPolygons, float size1, float size2);

#endif