#include <assert.h>

#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"

#include "mines.h"
#include "macros.h"

void mine_collisionBox_init(polygon* collision, 
    unsigned int nbPolygons /*SHALL NOT BE 0*/, float size1, float size2)
{
    collision->nbConvexPolygons = nbPolygons;
    collision->convexPolygons   = malloc(nbPolygons * sizeof(convexPolygon));

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

        collision->convexPolygons[i].size = 4;

        float angle = i * 2 * PI / nbPolygons;

        for (unsigned int j = 0; j < collision->convexPolygons[i].size; j++)
        {
            //RotatePoint is less peformant.
            //Since the center of rotation is (0;0) , we can use rotateVector instead.
            points[j] = rotateVector(points[j], angle);
        }

        collision->convexPolygons[i].points = points;
    }
}

//TODO : enum Mine Type
// void mine_spawn(void* mine, unsigned int type, vector2D spawner)
// {
//     if (type == 0)
//     {
//         t_mine* floatingMine = mine;
//         mine_init(floatingMine, false, false);
//         floatingMine->entity.ref.origin = spawner;
//         return;
//     }
//     if (type == 1)
//     {
//         t_mine* magneticMine = mine;
//         mine_init(magneticMine, true, false);
//         magneticMine->entity.ref.origin = spawner;
//         return;
//     }
//     if (type == 2)
//     {
//         t_mine* fireballMine = mine;
//         mine_init(fireballMine, false, true);
//         fireballMine->entity.ref.origin = spawner;
//         return;
//     }
//     if (type == 3)
//     {
//         t_mine* magneticFireballMine = mine;
//         mine_init(magneticFireballMine, true, true);
//         magneticFireballMine->entity.ref.origin = spawner;
//         return;
//     }

//     assert(false); //type input is invalid : type must be from 0 to 3
// }

//void mine_init(t_mine* mine, bool followPlayer, bool throwFireballs)
//void mine_spawn(void* mine, unsigned int type, vector2D location)
void mine_init(t_mine* mine, unsigned int type, vector2D location, float size)
{
    entity_init(&mine->entity);

    mine->entity.collision = malloc(sizeof(polygon));

    //floating mine
    switch (type)
    {
        case 0 :
            mine_collisionBox_init(mine->entity.collision, 3, 25, 100);
            mine->followPlayer = false;
            mine->throwFireballs = false;
            break;

        case 1 :
            mine_collisionBox_init(mine->entity.collision, 4, 50, 100);
            mine->followPlayer = true;
            mine->throwFireballs = false;
            break;
        
        case 2 :
            mine_collisionBox_init(mine->entity.collision, 4, 25, 100);
            mine->followPlayer = false;
            mine->throwFireballs = true;
            break;

        case 3 :
            mine_collisionBox_init(mine->entity.collision, 4, 10, 100);
            mine->followPlayer = true;
            mine->throwFireballs = true;
            break;
        default :
            assert(false); //no mine of this id exists
            break;
    }

    polygon* collision = mine->entity.collision;
    //TODO : resize Polygon (because same as player's one)
	for (unsigned int i = 0; i < collision->nbConvexPolygons; i++)
	{	
		for (unsigned int j = 0; j < collision->convexPolygons[i].size; j++)
		{
			collision->convexPolygons[i].points[j] = scaleVector(collision->convexPolygons[i].points[j], size);
		}
	}

    mine->size = size;
    mine->entity.ref.origin = location;
	mine->entity.collisionType = E_FLOATING_MINE;
    mine->target = NULL;
}

void mine_destroy(t_mine* mine)
{

}

void mine_render(t_mine* mine, t_render* render)
{
    entity_render(&mine->entity, render);
    polygon_render(render->renderer, &mine->entity.worldCollider, &mine->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, mine->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, mine->entity.aabb);
}

vector2D getShortestPath(vector2D a, vector2D b)
{
    if (vectorSquareLength(a) > vectorSquareLength(b)) //for optimization
        return b;
    else
        return a;
}

void mine_tick(t_mine* mine, float deltaTime)
{
    entity_tick(&mine->entity, deltaTime);

    if (mine->followPlayer)
    {
        //search for player
        if (mine->target != NULL)
        {
            vector2D mineLoc = mine->target->entity.ref.origin; 
            vector2D targetLoc = mine->entity.ref.origin;
            vector2D targetXP = addVectors(targetLoc, (vector2D) {WINDOW_SIZE_X, 0.f});
            vector2D targetXM = addVectors(targetLoc, (vector2D) {- WINDOW_SIZE_X, 0.f});
            vector2D targetYP = addVectors(targetLoc, (vector2D) {0.f, WINDOW_SIZE_Y});
            vector2D targetYM = addVectors(targetLoc, (vector2D) {0.f, - WINDOW_SIZE_Y});
            
            vector2D shortestPath = substractVectors(mineLoc, targetLoc);
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetXP));
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetXM));
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetYP));
            shortestPath = getShortestPath(shortestPath, substractVectors(mineLoc, targetYM));

            vector2D direction = nullVector();
            if (vectorLength(shortestPath) != 0.f)
            {
                direction = unitVector(shortestPath);
                mine->entity.ref.unitI = direction;
                mine->entity.ref.unitJ = rotateVector90(mine->entity.ref.unitI);
                //mine->entity.velocity = addVectors(mine->entity.velocity, scaleVector(direction, MAGNETIC_MINE_SPEED));
                entity_move(&mine->entity, E_FORWARD, deltaTime);
            }
            mine->entity.maxSpeed = MAGNETIC_MINE_SPEED;
        }
    }
    else
        mine->entity.velocity = scaleVector((vector2D) FLOATING_MINE_VELOCITY, 1 / mine->size / 4.f);
}