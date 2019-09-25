#ifndef _MINELAYER_H_
#define _MINELAYER_H_

#include "vector2D/vector2D.h"
#include "../mines.h"

void minelayer_collisionBox_init(polygon* collision);
convexPolygon minelayer_trunk_create();
convexPolygon minelayer_rightWing_create();
convexPolygon minelayer_leftWing_create();
convexPolygon minelayer_head_create();

void minelayer_create(t_dynamicArray* mines);

#endif