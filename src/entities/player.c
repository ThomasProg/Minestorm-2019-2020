#include <time.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "dynamicArray/dynamicArray.h"
#include "player.h"
#include "bullets.h"
#include "world.h"
#include "macros.h"

void player_collision_init(polygon* collision)
{
	collision->nbConvexPolygons = 2;
	collision->convexPolygons = malloc(collision->nbConvexPolygons * sizeof(convexPolygon));

	vector2D* points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0 * SHIP_SIZE, 50.0 * SHIP_SIZE};
	points[2] = (point2D) {100 * SHIP_SIZE, 0.0};
	collision->convexPolygons[0].size = 3;	
	collision->convexPolygons[0].points = points;	

	points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0 * SHIP_SIZE, -50.0 * SHIP_SIZE};
	points[2] = (point2D) {100.0 * SHIP_SIZE, 0.0};
	collision->convexPolygons[1].size = 3;	
	collision->convexPolygons[1].points = points;	
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

    player->entity.collision = malloc(sizeof(polygon));
    player_collision_init(player->entity.collision);
	player->entity.collisionType = E_PLAYER;

	player->life = PLAYER_LIFE;
	player->score = 0;
	player->lastShoot = 0;
	player->nextTeleportationDelay = 0.f;

	player->bInputs = bInput_create();
}

void player_destroy(t_player* player)
{
    polygon_free(player->entity.collision);
	free(player->entity.collision);
    polygon_free(&player->entity.worldCollider);
}

void player_render(t_player* player, t_render* render, bool renderDebug)
{
    entity_render(&player->entity, render, renderDebug);
	polygon_aabb_generate(&player->entity.worldCollider);
	SDL_SetRenderDrawColor(render->renderer, 100, 100, 255, 255);
    polygon_render(render->renderer, &player->entity.worldCollider, renderDebug);
}

void player_tick(t_player* player, float deltaTime)
{
    player_inputs_run(player, deltaTime);

    entity_tick(&player->entity, deltaTime);
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
	
	player->nextTeleportationDelay -= deltaTime;
	if (player->bInputs.teleport)
	{
		if (player->nextTeleportationDelay < 0)
		{
			player->nextTeleportationDelay = TELEPORTATION_DELAY;
			entity_teleport(&player->entity);
		}
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

void player_damages(t_player* player, t_dynamicArray* players, unsigned int id)
{
	if (player->life == 1)
	{
		player_destroy(player);
		dynamicArray_RemoveItem(players, id);
	}
	else
	{
		entity_teleport(&player->entity);
	}
	if (player->life != 0)
		player->life--;	
}