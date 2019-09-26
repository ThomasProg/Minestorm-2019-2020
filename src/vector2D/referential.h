#ifndef _REFERENTIAL_H_
#define _REFERENTIAL_H_

#include "vector2D.h"

/*
typedef struct 
{
    decimal referential;
    void* figure;
    int figureType;
} objects;
*/
typedef struct 
{
    vector2D origin;
    vector2D unitI;
    vector2D unitJ;
} referential;


point2D worldToLocal_point2D(point2D point1, referential ref);
point2D localToWorld_point2D(point2D point1, referential ref);

vector2D worldToLocal_vector2D(vector2D vector1, referential ref);
vector2D localToWorld_vector2D(vector2D vector1, referential ref);


convexPolygon localToWorld_convexPolygon(convexPolygon* convexPolygon1, referential ref);
polygon localToWorld_polygon            (polygon* polygon1, referential ref);

#endif