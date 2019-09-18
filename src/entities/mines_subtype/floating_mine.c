#include <time.h>

#include "floating_mine.h"
#include "macros.h"

t_floatingMine* floatingMine_create()
{
	t_floatingMine* floatingMine = malloc(sizeof(*floatingMine));

    entity_init(&floatingMine->entity);

    floatingMine->entity.collision = malloc(sizeof(convexPolygonsArray));
    mine_collisionBox_init(floatingMine->entity.collision, 3, 25, 100);

	floatingMine->entity.collisionType = E_FLOATING_MINE;

	return floatingMine;
}

void floatingMine_render(t_floatingMine* floatingMine, t_render* render)
{
    entity_render(&floatingMine->entity, render);
    convexPolygonsArray_render(render->renderer, floatingMine->entity.collision, &floatingMine->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, floatingMine->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, floatingMine->entity.aabb);
}

void floatingMine_tick(t_floatingMine* floatingMine, float deltaTime)
{
    entity_tick(&floatingMine->entity, deltaTime);
    //floatingMine->entity.ref.unitI = (vector2D) {4.f, 3.f};
    //floatingMine->entity.ref.unitJ = rotateVector90(floatingMine->entity.ref.unitI);
    floatingMine->entity.velocity = (vector2D) {0.1f, 0.3f};
}