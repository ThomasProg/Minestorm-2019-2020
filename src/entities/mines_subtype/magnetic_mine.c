#include <stdio.h>
#include <time.h>

#include "magnetic_mine.h"
#include "macros.h"

void magneticMine_init(t_magneticMine* magneticMine)
{
    entity_init(&magneticMine->entity);
    magneticMine->entity.maxSpeed = 200.f;

    magneticMine->entity.ref.origin = (vector2D) {100.f, 100.f};

    magneticMine->entity.collision = malloc(sizeof(polygon));
    mine_collisionBox_init(magneticMine->entity.collision, 4, 25, 100);

	magneticMine->entity.collisionType = E_MAGNETIC_MINE;
    magneticMine->target = NULL;
}

void magneticMine_render(t_magneticMine* magneticMine, t_render* render)
{
    entity_render(&magneticMine->entity, render);
    polygon_render(render->renderer, &magneticMine->entity.worldCollider, &magneticMine->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, floatingMine->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, floatingMine->entity.aabb);
}

vector2D getShortestVector(vector2D a, vector2D b)
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
        shortestPath = getShortestVector(shortestPath, substractVectors(mineLoc, targetXP));
        shortestPath = getShortestVector(shortestPath, substractVectors(mineLoc, targetXM));
        shortestPath = getShortestVector(shortestPath, substractVectors(mineLoc, targetYP));
        shortestPath = getShortestVector(shortestPath, substractVectors(mineLoc, targetYM));

        vector2D direction = nullVector();
        if (vectorLength(shortestPath) != 0.f)
        {
            direction = unitVector(shortestPath);
            magneticMine->entity.ref.unitI = direction;
            magneticMine->entity.ref.unitJ = rotateVector90(magneticMine->entity.ref.unitI);
            //magneticMine->entity.velocity = addVectors(magneticMine->entity.velocity, scaleVector(direction, MAGNETIC_MINE_SPEED));
            entity_move(&magneticMine->entity, E_FORWARD, deltaTime);
        }
    }
}