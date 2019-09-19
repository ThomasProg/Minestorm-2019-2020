#include <time.h>

#include "fireball_mine.h"
#include "macros.h"

void fireballMine_init(t_fireballMine* fireballMine)
{
    entity_init(&fireballMine->entity);

    fireballMine->entity.ref.origin = (vector2D) {500.f, 100.f};

    fireballMine->entity.collision = malloc(sizeof(polygon));
    mine_collisionBox_init(fireballMine->entity.collision, 4, 50, 100);

	fireballMine->entity.collisionType = E_MAGNETIC_MINE;
}

void fireballMine_render(t_fireballMine* fireballMine_create, t_render* render)
{
    entity_render(&fireballMine_create->entity, render);
    polygon_render(render->renderer, &fireballMine_create->entity.worldCollider, &fireballMine_create->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, floatingMine->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, floatingMine->entity.aabb);
}

void fireballMine_tick(t_fireballMine* fireballMine_create, float deltaTime)
{
    entity_tick(&fireballMine_create->entity, deltaTime);
}