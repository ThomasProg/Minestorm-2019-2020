#include <time.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "player.h"
#include "bullets.h"
#include "macros.h"

void player_collision_init(polygon* collision)
{
	collision->nbConvexPolygons = 2;
	collision->convexPolygons = malloc(collision->nbConvexPolygons * sizeof(convexPolygon));

	vector2D* points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, 50.0};
	points[2] = (point2D) {100, 0.0};
	collision->convexPolygons[0].size = 3;	
	collision->convexPolygons[0].points = points;	

	points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, -50.0};
	points[2] = (point2D) {100.0, 0.0};
	collision->convexPolygons[1].size = 3;	
	collision->convexPolygons[1].points = points;	

	//scale convexPolygonsArray
	for (unsigned int i = 0; i < collision->nbConvexPolygons; i++)
	{	
		for (unsigned int j = 0; j < collision->convexPolygons[i].size; j++)
		{
			collision->convexPolygons[i].points[j] = scaleVector(collision->convexPolygons[i].points[j], SHIP_SIZE);
		}
	}
}

bInputs bInput_create()
{
	bInputs bInputs;
	bInputs.isForward = false;
	bInputs.isLeft = false;
	bInputs.isRight = false;
	bInputs.shoot = false;
	bInputs.teleport = false;

	return bInputs;
}

inputValues getInputValues(unsigned int id)
{
	inputValues inputValues1;
	switch (id)
	{
		case 0 : 
			inputValues1 = getInputValues1();
			break;

		case 1 : 
			inputValues1 = getInputValues2();
			break;

		default :
			break;
	}
	return inputValues1;
}

inputValues getInputValues1()
{
	inputValues inputValues;
	inputValues.forward = SDLK_r;
	inputValues.shoot = SDLK_f;
	inputValues.right = SDLK_g;
	inputValues.left = SDLK_d;
	inputValues.teleport1 = SDLK_e;
	inputValues.teleport2 = SDLK_t;

	return inputValues;
}

inputValues getInputValues2()
{
	inputValues inputValues;
	inputValues.forward = SDLK_i;
	inputValues.shoot = SDLK_k;
	inputValues.right = SDLK_l;
	inputValues.left = SDLK_j;
	inputValues.teleport1 = SDLK_u;
	inputValues.teleport2 = SDLK_o;

	return inputValues;
}

void player_init(t_player* player)
{
    entity_init(&player->entity);
	dynamicArray_Init(&player->bullets, sizeof(t_bullet), 16);

    player->entity.collision = malloc(sizeof(polygon));
    player_collision_init(player->entity.collision);
	player->entity.collisionType = E_PLAYER;

	player->life = 0;
	player->score = 0;

	player->bInputs = bInput_create();
}

void player_destroy(t_player* player)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&player->bullets, &i); i++)
	{
		bullet_destroy(dynamicArray_GetItem(&player->bullets, i)); 
	}

	dynamicArray_Destroy(&player->bullets);
}

void player_render(t_player* player, t_render* render)
{
    entity_render(&player->entity, render);
    polygon_render(render->renderer, &player->entity.worldCollider, &player->entity.ref);

	// for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&player->bullets, &i); i++)
	// {
	// 	bullet_render(render->renderer, dynamicArray_GetItem(&player->bullets, i), BULLETS_PRECISION);
	// }

    //printf("x : %f y : %f\n", player->entity.ref.origin.x, player->entity.ref.origin.y);
	//printf("x : %f y : %f\n", player->entity.velocity.x, player->entity.velocity.y);

	polygon* polygon = &player->entity.worldCollider;
	point2D* points = (polygon->convexPolygons[0]).points;//, polygon->convexPolygons->size);
    polygon->convexPolygons[0].aabb = aabbRectangleGenerate(points, polygon->convexPolygons[0].size);
    axisAlignedRectangle_render(render->renderer, polygon->convexPolygons[0].aabb);

	polygon = &player->entity.worldCollider;
	points = (polygon->convexPolygons[1]).points;//, polygon->convexPolygons->size);
    polygon->convexPolygons[1].aabb = aabbRectangleGenerate(points, polygon->convexPolygons[1].size);
    axisAlignedRectangle_render(render->renderer, polygon->convexPolygons[1].aabb);

	polygon->aabb = fuseAxisAlignedRectangles(polygon->convexPolygons[0].aabb, polygon->convexPolygons[1].aabb);
	axisAlignedRectangle_render(render->renderer, polygon->aabb);
}

void player_tick(t_player* player, float deltaTime)
{
    player_inputs_run(player, deltaTime);

    entity_tick(&player->entity, deltaTime);

	// t_bullet* bullet;
	// for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&player->bullets, &i); i++)
	// {
	// 	bullet = dynamicArray_GetItem(&player->bullets, i);

	// 	bullet_tick(bullet, deltaTime);
	// 	if (bullet->timeAlive > BULLETS_TIME_ALIVE)
	// 	{
	// 		bullet_destroy(bullet);
    //    		dynamicArray_RemoveItem(&player->bullets, i);
	// 	}
	// }
}

void player_inputs_run(t_player* player, float deltaTime)
{
	if (player->bInputs.isForward)
	{
		entity_move(&player->entity, E_FORWARD, deltaTime);
	}

	if (player->bInputs.isLeft)
	{
		entity_move(&player->entity, E_LEFT, deltaTime);
	}

	if (player->bInputs.isRight)
	{
		entity_move(&player->entity, E_RIGHT, deltaTime);
	}
	
	if (player->bInputs.teleport)
	{
		entity_teleport(&player->entity);
	}
}

void player_input_start(t_player* player, int key, bool start)
{
	if (key == player->inputValues.forward)
	{
		player->bInputs.isForward = start;
	}		
	if (key == player->inputValues.left)
	{
		player->bInputs.isLeft = start;
	}
	if (key == player->inputValues.right)
	{
		player->bInputs.isRight = start;
	}
		
	if (key == player->inputValues.teleport1 || key == player->inputValues.teleport2)
	{
		player->bInputs.teleport = start;
	}

	if (key == player->inputValues.shoot)
	{
		player->bInputs.shoot = start;
	}
}

