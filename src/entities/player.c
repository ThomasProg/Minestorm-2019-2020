#include <time.h>

#include "player.h"
#include "bullets.h"
#include "macros.h"

void player_collision_init(convexPolygonsArray* collision)
{
	collision->numberOfPolygons = 2;
	collision->polygons = malloc(collision->numberOfPolygons * sizeof(convexPolygon));

	vector2D* points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, 50.0};
	points[2] = (point2D) {100, 0.0};
	collision->polygons[0].size = 3;	
	collision->polygons[0].points = points;	

	points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, -50.0};
	points[2] = (point2D) {100.0, 0.0};
	collision->polygons[1].size = 3;	
	collision->polygons[1].points = points;	
}

inputs input_create()
{
	inputs inputs;
	inputs.isForward = false;
	inputs.isLeft = false;
	inputs.isRight = false;

	return inputs;
}

t_player* player_create()
{
	t_player* player = malloc(sizeof(*player));

    entity_init(&player->entity);

    player->entity.collision = malloc(sizeof(convexPolygonsArray));
    player_collision_init(player->entity.collision);
	player->entity.collisionType = E_PLAYER;

	player->life = 0;
	player->score = 0;
	player->bullet = false;

	player->inputs = input_create();

	return player;
}

void player_render(t_player* player, t_render* render)
{
    entity_render(&player->entity, render);
    convexPolygonsArray_render(render->renderer, player->entity.collision, &player->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, player->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, player->entity.aabb);
}

void player_tick(t_player* player, float deltaTime)
{
    player_inputs_run(player, deltaTime);
    entity_tick(&player->entity, deltaTime);
}

void player_inputs_run(t_player* player, float deltaTime)
{
	if (player->inputs.isForward)
	{
		entity_move(&player->entity, E_FORWARD, deltaTime);
	}

	if (player->inputs.isLeft)
	{
		entity_move(&player->entity, E_LEFT, deltaTime);
	}

	if (player->inputs.isRight)
	{
		entity_move(&player->entity, E_RIGHT, deltaTime);
	}
}

void player_input_start(t_player* player, int key, bool start)
{
	switch (key)
	{
		case SDLK_r :
			player->inputs.isForward = start;
			break;
		
		case SDLK_d :
			player->inputs.isLeft = start;
			break;
		case SDLK_g :
			player->inputs.isRight = start;
			break;
		
		case SDLK_e :
			entity_teleport(&player->entity);
			break;

		case SDLK_t :
			entity_teleport(&player->entity);
			break;

		case SDLK_f :
			player->bullet = true;
			break;
	}
}

void player_event(t_player* player, t_bullet** bullets)
{
	if (player->bullet)
	{
		vector2D bulletLocation = player->entity.ref.origin;
		bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0));
		bullets[0] = bullet_create(bulletLocation, player->entity.ref.unitI);
		player->bullet = false;
	}
}