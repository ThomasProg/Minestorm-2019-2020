#include <stdlib.h>
#include "minelayer.h"
#include "macros.h"

convexPolygon minelayer_trunk_create()
{
    convexPolygon returned;

    vector2D* points = malloc(4 * sizeof(point2D));
    points[0] = (point2D) {-40.f, -5.0};
    points[1] = (point2D) {-40.f, 5.0};
    points[2] = (point2D) {40.f, 5.0};
    points[3] = (point2D) {40.f, -5.0};
    returned.size = 4;
    returned.points = points;

    return returned;
}

convexPolygon minelayer_rightWing_create()
{
    convexPolygon returned;

    vector2D* points = malloc(3 * sizeof(point2D));
    points[0] = (point2D) {40.f, -5.0};
    points[1] = (point2D) {50.f, 10.0};
    points[2] = (point2D) {40.f, 5.0};
    returned.size = 3;
    returned.points = points;

    return returned;
}

convexPolygon minelayer_leftWing_create()
{
    convexPolygon returned;

    vector2D* points = malloc(3 * sizeof(point2D));
    points[0] = (point2D) {-40.f, -5.0};
    points[1] = (point2D) {-50.f, 10.0};
    points[2] = (point2D) {-40.f, 5.0};
    returned.size = 3;
    returned.points = points;

    return returned;
}

convexPolygon minelayer_head_create()
{
    convexPolygon returned;

    vector2D* points = malloc(3 * sizeof(point2D));
    points[0] = (point2D) {-10.f, -5.0};
    points[1] = (point2D) {0.f, -10.0};
    points[2] = (point2D) {10.f, -5.0};
    returned.size = 3;
    returned.points = points;

    return returned;
}

void minelayer_collisionBox_init(polygon* collision)
{
    unsigned int nbPolygons = 4;
    collision->nbConvexPolygons = nbPolygons;
    collision->convexPolygons   = malloc(nbPolygons * sizeof(convexPolygon));

    //trunk
    collision->convexPolygons[0] = minelayer_trunk_create();
    //left wing
    collision->convexPolygons[1] = minelayer_leftWing_create();
    //right wing
    collision->convexPolygons[2] = minelayer_rightWing_create();
    //head
    collision->convexPolygons[3] = minelayer_head_create();
}

vector2D minelayer_getSpawnLocation()
{
    vector2D loc;
    unsigned int spawnLocID = rand() % 4;
    //abscisse
    if (spawnLocID % 2 == 0)
    {
        loc.x = rand () % WINDOW_SIZE_X;
        if (spawnLocID == 0)
            loc.y = 0;
        else
            loc.y = WINDOW_SIZE_Y;
    }
    //ordinates
    else
    {
        loc.y = rand () % WINDOW_SIZE_Y;
        if (spawnLocID == 1)
            loc.x = 0;
        else
            loc.x = WINDOW_SIZE_X;
    }

    return loc;
}

void minelayer_create(t_dynamicArray* mines)
{
    t_mine* mine = dynamicArray_AddItem(mines);
    vector2D loc = minelayer_getSpawnLocation();
    
    mine_init(mine, 4, loc, BIG);
}