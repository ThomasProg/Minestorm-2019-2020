#include <time.h>

#include "fireball_mine.h"
#include "macros.h"

t_fireballMine* fireballMine_create()
{
	t_fireballMine* fireballMine_create = malloc(sizeof(*fireballMine_create));

    entity_init(&fireballMine_create->entity);

    fireballMine_create->entity.ref.origin = (vector2D) {500.f, 100.f};

    fireballMine_create->entity.collision = malloc(sizeof(convexPolygonsArray));
    mine_collisionBox_init(fireballMine_create->entity.collision, 4, 50, 100);

	fireballMine_create->entity.collisionType = E_MAGNETIC_MINE;

	return fireballMine_create;
}

void fireballMine_render(t_fireballMine* fireballMine_create, t_render* render)
{
    entity_render(&fireballMine_create->entity, render);
    convexPolygonsArray_render(render->renderer, fireballMine_create->entity.collision, &fireballMine_create->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, floatingMine->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, floatingMine->entity.aabb);
}

void fireballMine_tick(t_fireballMine* fireballMine_create, float deltaTime)
{
    entity_tick(&fireballMine_create->entity, deltaTime);
}