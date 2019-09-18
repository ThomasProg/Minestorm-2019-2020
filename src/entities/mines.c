#include <assert.h>

#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"

#include "mines.h"
#include "macros.h"

void mine_collisionBox_init(convexPolygonsArray* collision, 
    unsigned int nbPolygons /*SHALL NOT BE 0*/, float size1, float size2)
{
    collision->numberOfPolygons = nbPolygons;
    collision->polygons         = malloc(nbPolygons * sizeof(convexPolygon));

    //set each polygon of the star
    for (unsigned int i = 0; i < nbPolygons; i++)
    {
        vector2D* points = malloc(4 * sizeof(point2D));

        vector2D temp = {size1, 0.f};
        temp = rotateVector(temp, - PI/nbPolygons + 2 * PI / nbPolygons);

        points[0] = temp;
        points[1] = (point2D) {size2, 0.0};
        temp.y   *= -1;
        points[2] = temp;
        points[3] = (point2D) {0.0, -0.0};

        collision->polygons[i].size = 4;

        float angle = i * 2 * PI / nbPolygons;

        for (unsigned int j = 0; j < collision->polygons[i].size; j++)
        {
            //RotatePoint is less peformant.
            //Since the center of rotation is (0;0) , we can use rotateVector instead.
            points[j] = rotateVector(points[j], angle);
        }

        collision->polygons[i].points = points;
    }
}

//TODO : enum Mine Type
void* mine_spawn(vector2D* spawners, unsigned int nbSpawners, int type)
{
    if (type == 0)
    {
        t_floatingMine* floatingMine;
        floatingMine = floatingMine_create();
        floatingMine->entity.ref.origin = spawners[nbSpawners];
        return floatingMine;
    }
    if (type == 1)
    {
        t_magneticMine* magneticMine;
        magneticMine = magneticMine_create();
        magneticMine->entity.ref.origin = spawners[nbSpawners];
        return magneticMine;
    }
    if (type == 2)
    {
        t_fireballMine* fireballMine;
        fireballMine = fireballMine_create();
        fireballMine->entity.ref.origin = spawners[nbSpawners];
        return fireballMine;
    }

    assert(true); //type input is invalid : type must be from 0 to 3
    return NULL;
}