#include <time.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

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

	//scale convexPolygonsArray
	for (unsigned int i = 0; i < collision->numberOfPolygons; i++)
	{	
		for (unsigned int j = 0; j < collision->polygons[i].size; j++)
		{
			collision->polygons[i].points[j] = scaleVector(collision->polygons[i].points[j], SHIP_SIZE);
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

	return bInputs;
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

t_player* player_create()
{
	t_player* player = malloc(sizeof(t_player));

    entity_init(&player->entity);

    player->entity.collision = malloc(sizeof(convexPolygonsArray));
    player_collision_init(player->entity.collision);
	player->entity.collisionType = E_PLAYER;

	player->life = 0;
	player->score = 0;

	player->nbBullets = 0;
	player->bullets = NULL;

	player->bInputs = bInput_create();

	return player;
}

void player_destroy(t_player* player)
{
	for (unsigned int i = 0; i < player->nbBullets; i++)
		bullet_destroy(player->bullets[i]);

	free(player->bullets);

	free(player);
}

void player_render(t_player* player, t_render* render)
{
    entity_render(&player->entity, render);
    convexPolygonsArray_render(render->renderer, player->entity.collision, &player->entity.ref);

	for (unsigned int i = 0; i < player->nbBullets; i++)
	{
		bullet_render(render->renderer, player->bullets[i], BULLETS_PRECISION);
	}

    //printf("x : %f y : %f\n", player->entity.ref.origin.x, player->entity.ref.origin.y);
	//printf("x : %f y : %f\n", player->entity.velocity.x, player->entity.velocity.y);

    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    // axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    // rec.center = addVectors(rec.center, player->entity.ref.origin); 
    // axisAlignedRectangle_render(render->renderer, player->entity.aabb);
}

void player_tick(t_player* player, float deltaTime)
{
    player_inputs_run(player, deltaTime);

    entity_tick(&player->entity, deltaTime);

	for (unsigned int i = 0; i < player->nbBullets; i++)
	{
		bullet_tick(player->bullets[i], deltaTime);
	}
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

	// if (player->bInputs.shoot)
	// {
	// 	entity_move(&player->entity, -1, deltaTime);
	// }
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
		entity_teleport(&player->entity);
	}

	if (key == player->inputValues.shoot)
	{
		vector2D bulletLocation = player->entity.ref.origin;
		bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0 * SHIP_SIZE));

		t_bullet* bullet = NULL;
		if (player->bullets == NULL && player->nbBullets == 0)
		{
			player->nbBullets = 1;
			player->bullets = malloc(sizeof(t_bullet*));
			bullet = bullet_create();
			player->bullets[0] = bullet;
		}
		else
		{	
			unsigned int i = 0;
			while (i < player->nbBullets)
			{
				if (!player->bullets[i]->isAlive)
				{
					bullet = player->bullets[i];
					break;
				}
				i++;
			}
			if (bullet == NULL)
			{
				unsigned int currentLoc = i;
				player->nbBullets *= 2;
				player->bullets = realloc(player->bullets, player->nbBullets * sizeof(t_bullet*));
				while (i < player->nbBullets)
				{
					player->bullets[i] = bullet_create();
					i++;
				}
				bullet = player->bullets[currentLoc];
			}
		}
		player->bullets[player->nbBullets] = bullet_create(bulletLocation, player->entity.ref.unitI);
		bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI);
		player->bInputs.shoot = false;
	}
}

void player_event(t_player* player, t_bullet** bullets)
{
	/*
	if (player->bullet)
	{
		vector2D bulletLocation = player->entity.ref.origin;
		bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0));
		bullets[0] = bullet_create(bulletLocation, player->entity.ref.unitI);
		player->bullet = false;
	}
	*/
}