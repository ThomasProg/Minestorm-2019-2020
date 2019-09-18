#include <stdio.h>
#include <time.h>

#include "magnetic_mine.h"
#include "macros.h"

t_magneticMine* magneticMine_create()
{
	t_magneticMine* magneticMine = malloc(sizeof(*magneticMine));

    entity_init(&magneticMine->entity);

    magneticMine->entity.ref.origin = (vector2D) {100.f, 100.f};

    magneticMine->entity.collision = malloc(sizeof(convexPolygonsArray));
    mine_collisionBox_init(magneticMine->entity.collision, 4, 25, 100);

	magneticMine->entity.collisionType = E_MAGNETIC_MINE;
    magneticMine->target = NULL;

	return magneticMine;
}

void magneticMine_render(t_magneticMine* magneticMine, t_render* render)
{
    entity_render(&magneticMine->entity, render);
    convexPolygonsArray_render(render->renderer, magneticMine->entity.collision, &magneticMine->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, floatingMine->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, floatingMine->entity.aabb);
}

vector2D getShortestPath(vector2D a, vector2D b)
{
    if (vectorLength(a) > vectorLength(b))
        return b;
    else
        return a;
}

void magneticMine_tick(t_magneticMine* magneticMine, float deltaTime)
{
    entity_tick(&magneticMine->entity, deltaTime);

    //search for player
    if (magneticMine->target != NULL)
    {
        vector2D mineLoc = magneticMine->target->entity.ref.origin; 
        vector2D targetLoc = magneticMine->entity.ref.origin;
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
            direction = unitVector(shortestPath);

        magneticMine->entity.velocity = scaleVector(direction, MAGNETIC_MINE_SPEED);
    }
}