#include <assert.h>

#include "world.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"
#include "entities/spawner.h"
#include "worldField.h"
#include "entities/bullets.h"
#include "vector2D/collisions2D.h"
#include "macros.h"

t_world* world_create()
{
    t_world* world = malloc(sizeof(t_world));

	dynamicArray_Init(&world->players, sizeof(t_player), 2);

	for (unsigned int i = 0; i < 2; i++)
	{
		t_player* player = dynamicArray_AddItem(&world->players);

		player_init(player);
		player->inputValues = getInputValues(i % 2);
	}

	dynamicArray_Init(&world->mines, sizeof(t_mine), 4);
	dynamicArray_Init(&world->bullets, sizeof(t_bullet), 16);
	// dynamicArray_Init(&world->floatingMines, sizeof(t_floatingMine), 4);
	// dynamicArray_Init(&world->magneticMines, sizeof(t_magneticMine), 4);
	dynamicArray_Init(&world->spawners, sizeof(t_spawner), 4);

	world->isPaused = false;

	newWorldLevel(world, 1);

    return world;
}

void world_destroy(t_world* world)
{
	dynamicArray_Destroy(&world->mines);
	//dynamicArray_Destroy(&world->floatingMines);
	//dynamicArray_Destroy(&world->magneticMines);
	//dynamicArray_Destroy(&world->fireballMines);

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		player_destroy(dynamicArray_GetItem(&world->players, i));
	}
	dynamicArray_Destroy(&world->bullets);
	dynamicArray_Destroy(&world->spawners);
	dynamicArray_Destroy(&world->players);

    free(world);
}

void newWorldLevel(t_world* world, unsigned int level)
{
	//set spawners
	unsigned int nbSpawners = pow((level % 3), 1.1) + 2 + (level / 3);
	nbSpawners = 5;

	t_spawner* spawner;

	for (unsigned int i = 0; i < nbSpawners; i++)
	{
		spawner = dynamicArray_AddItem(&world->spawners);
		spawner->location = getRandomLocation();
		spawner->size = 0.8f;
	}
}

void world_render(t_world* world, t_assets* assets)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, i);
		mine_render(mine, assets->render);
	}
	// for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	// {
	// 	t_floatingMine* floatingMine = dynamicArray_GetItem(&world->mines, i);
	// 	floatingMine_render(floatingMine, assets->render);
	// }

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		player_render(player, assets->render);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->bullets, &i); i++)
	{
		t_bullet* bullet = dynamicArray_GetItem(&world->bullets, i);
		bullet_render(assets->render->renderer, bullet, BULLETS_PRECISION);
	}
}

void world_tick(t_world* world, float deltaTime)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		player_tick(player, deltaTime);

		if (player->bInputs.shoot)
		{
			vector2D bulletLocation = player->entity.ref.origin;
			bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0 * 0.25 + 10));

			t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

			bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI);
			player->bInputs.shoot = false;
		}
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, i);
		mine_tick(mine, deltaTime);

		// if (mine->shoot)
		// {
		// 	vector2D bulletLocation = mine->entity.ref.origin;
		// 	bulletLocation = addVectors(bulletLocation, scaleVector(mine->entity.ref.unitI, 100.0 * 0.25 + 10));

		// 	t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

		// 	bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI);
		// 	player->bInputs.shoot = false;
		// }
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->bullets, &i); i++)
	{
		t_bullet* bullet = dynamicArray_GetItem(&world->bullets, i);
		bullet_tick(bullet, deltaTime);

		bullet = dynamicArray_GetItem(&world->bullets, i);

		bullet_tick(bullet, deltaTime);
		if (bullet->timeAlive > BULLETS_TIME_ALIVE)
		{
			bullet_destroy(bullet);
			dynamicArray_RemoveItem(&world->bullets, i);
		}
	}

	// for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->floatingMines, &i); i++)
	// {
	// 	t_floatingMine* floatingMine = dynamicArray_GetItem(&world->floatingMines, i);
	// 	floatingMine_tick(floatingMine, deltaTime);
	// }

	// for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->magneticMines, &i); i++)
	// {
	// 	t_magneticMine* magneticMine = dynamicArray_GetItem(&world->magneticMines, i);
	// 	magneticMine_tick(magneticMine, deltaTime);
	// 	magneticMine->target = dynamicArray_GetItem(&world->players, 0);
	// }
}

void world_inputs(game* game, t_world* world)
{
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN) //press
		{
			int key = event.key.keysym.sym;
			if (key == SDLK_SPACE)
			{
				world->isPaused = !world->isPaused;
				break;
			}

			if (key == SDLK_ESCAPE)
				game->run = false;

			for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
			{
				player_input_start(dynamicArray_GetItem(&world->players, i), key, true);
			}
		}
		if (event.type == SDL_KEYUP) //release
		{
			int key = event.key.keysym.sym;


			for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
			{
				player_input_start(dynamicArray_GetItem(&world->players, i), key, false);
			}
		}
		if (event.type == SDL_QUIT)
			game->run = false;
	}
}

t_bullet* bullets_polygon_collision(t_dynamicArray* bullets, polygon* polygon, unsigned int* bulletID)
{
	for (unsigned int k = 0; dynamicArray_GetValidItemIndex(bullets, &k); k++)
	{
		t_bullet* bullet = dynamicArray_GetItem(bullets, k);
		if (circle_polgyon_collision(&bullet->collision, polygon))
		{
			if (bulletID != NULL)
				*bulletID = k;

			return bullet;
		}
	}

	return NULL;
}

void world_collisions(t_world* world)
{
	for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->mines, &j); j++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, j);

		unsigned int bulletID = 0;
		t_bullet* bullet = bullets_polygon_collision(&world->bullets, &mine->entity.worldCollider, &bulletID);
		
		if (bullet != NULL)
		{
			bullet_destroy(bullet);
			dynamicArray_RemoveItem(&world->bullets, bulletID);

			if (mine->throwFireballs)
			{
				vector2D bulletLocation = mine->entity.ref.origin;
				bulletLocation = addVectors(bulletLocation, scaleVector(mine->entity.ref.unitI, 100.0 * 0.25 + 10));

				t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

				bullet_init(bullet, bulletLocation, mine->entity.velocity, mine->entity.ref.unitI);
			}

			if (mine->size > 0.3)
			{
				t_spawner* spawner;
				for (unsigned int i = 0; i < 2; i++)
				{
					spawner = dynamicArray_AddItem(&world->spawners);
					spawner->location = getRandomLocation();
					if (mine->size > 0.0)
						spawner->size = mine->size - 0.3f;
				}
			}

			mine_destroy(mine);

			dynamicArray_RemoveItem(&world->mines, j);

			//player->score += 100;
		}
	}

	// for (unsigned int k = 0; dynamicArray_GetValidItemIndex(&world->bullets, &k); k++)
	// {
	// 	t_bullet* bullet = dynamicArray_GetItem(&world->bullets, k);
	// 	if (circle_polgyon_collision(&bullet->collision, &entity->worldCollider))
	// 	{
	// 		bullet_destroy(bullet);
	// 		dynamicArray_RemoveItem(&player->bullets, k);

	// 		if (mine->bShootFireballs)
	// 		{
	// 			vector2D bulletLocation = player->entity.ref.origin;
	// 			bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0 * 0.25 + 10));

	// 			t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

	// 			bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI);
	// 			player->bInputs.shoot = false;
	// 		}
			
	// 		mine_destroy(mine);
	// 		dynamicArray_RemoveItem(&world->mines, j);

	// 		player->score += 100;
	// 	}
	// }

	//for each player
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		
		for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->mines, &j); j++)
		{
			t_mine* mine = dynamicArray_GetItem(&world->mines, j);
			t_entity* entity = &mine->entity;
			
			if (polygon_polgyon_collision(&player->entity.worldCollider, &entity->worldCollider))
			{
				//fprintf(stderr, "OOH WILFRE!\n");
			}
			else
			{
				//fprintf(stderr, "no collision !\n");
			}

			// for (unsigned int k = 0; dynamicArray_GetValidItemIndex(&player->bullets, &k); k++)
			// {
			// 	t_bullet* bullet = dynamicArray_GetItem(&player->bullets, k);
			// 	if (circle_polgyon_collision(&bullet->collision, &entity->worldCollider))
			// 	{
			// 		bullet_destroy(bullet);
			// 		dynamicArray_RemoveItem(&player->bullets, k);

			// 		if (mine->bShootFireballs)
			// 		{
			// 			vector2D bulletLocation = player->entity.ref.origin;
			// 			bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0 * 0.25 + 10));

			// 			t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

			// 			bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI);
			// 			player->bInputs.shoot = false;
			// 		}
					
			// 		mine_destroy(mine);
			// 		dynamicArray_RemoveItem(&world->mines, j);

			// 		player->score += 100;
			// 	}
			// }
		}
	}
}

void world_loop(t_assets* assets, float deltaTime, game* game)
{
    t_world* world = ((t_world*) (game->level.data));

	if (world->spawners.usedItems > 0)
	{
		unsigned int i = 0;
		dynamicArray_GetValidItemIndex(&world->spawners, &i);
		t_spawner* spawner = dynamicArray_GetItem(&world->spawners, i);

		//if (world->spawners.usedItems < 2)
		//{
			t_mine* mine = dynamicArray_AddItem(&world->mines);
			mine_init(mine, i % 4, spawner->location, spawner->size);
			mine->target = dynamicArray_GetItem(&world->players, 0);
			// if (world->spawners.usedItems < 2)
			// 	mine_spawn(mine, 0, *spawner);
			// else
			// {
			// 	mine_spawn(mine, 1, *spawner);
			// 	mine->target = dynamicArray_GetItem(&world->players, 0);
			// }
			
		//}

		// if (world->spawners.usedItems < 3)
		// {
		// 	t_floatingMine* mine = dynamicArray_AddItem(&world->floatingMines);
		// 	mine_spawn(mine, 0, *spawner);
		// }
		// else
		// {
		// 	t_floatingMine* mine = dynamicArray_AddItem(&world->magneticMines);
		// 	mine_spawn(mine, 1, *spawner);
		// }
		
		dynamicArray_RemoveItem(&world->spawners, i);
	 }

	world_render(world, assets);

	world_inputs(game, world);

	if (world->isPaused)
		return;

	world_tick(world, deltaTime);

	world_collisions(world);
}