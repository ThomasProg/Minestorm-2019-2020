#include <stdlib.h>
#include "world.h"
#include "entities/spawner.h"
#include "minelayer.h"
#include "worldField.h"
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

void minelayer_tick(t_world* world, t_mine* mine, unsigned int mineID, float deltaTime)
{
    if (mine->mineLayerDelay < 0.f)
    {
        t_spawner* spawner;
        spawner = dynamicArray_AddItem(&world->spawners);
        spawner->location = mine->entity.ref.origin;
        spawner->size = BIG;
        mine->mineLayerDelay = MINELAYER_ACTION_DELAY;
        world->spawnDelay = 2.f;

        unsigned int newSize = SMALL;
        unsigned int currentID = rand() % 4;
        if (currentID == 0)
            world->minesBySize[newSize].floatingMine++;
        if (currentID == 1)
            world->minesBySize[newSize].magneticMine++;
        if (currentID == 2)
            world->minesBySize[newSize].fireballMine++;
        if (currentID == 3)
            world->minesBySize[newSize].magneticFireballMine++;
    }
    else
        mine->mineLayerDelay -= deltaTime;

    if (border_teleportation(&mine->entity.ref.origin))
    {
        mine_destroy(mine);
        dynamicArray_RemoveItem(&world->mines, mineID);
    }
}

void minelayer_init(t_mine* mine, E_SIZE sizeType)
{
    minelayer_collisionBox_init(mine->entity.collision);
    mine->followPlayer = false;
    mine->throwFireballs = false;
    mine->isMineLayer = true;
    mine->sizeType = BIG;
    mine->givenScore = 1000;
    mine->entity.isTeleportingAtBorder = false;
}