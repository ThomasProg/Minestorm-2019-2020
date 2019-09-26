#ifndef _MINELAYER_H_
#define _MINELAYER_H_

#include "vector2D/vector2D.h"
#include "../mines.h"

typedef struct s_world t_world;

void minelayer_collisionBox_init(polygon* collision);
convexPolygon minelayer_trunk_create();
convexPolygon minelayer_rightWing_create();
convexPolygon minelayer_leftWing_create();
convexPolygon minelayer_head_create();

//DO NOT CALL//CALLED BY mine_init/////////////////////
void minelayer_create(t_dynamicArray* mines);        //
void minelayer_init(t_mine* mine, E_SIZE sizeType);  //
///////////////////////////////////////////////////////

vector2D minelayer_getSpawnLocation();
void minelayer_tick(t_world* world, t_mine* mine, unsigned int mineID, float deltaTime);

#endif