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
	dynamicArray_Init(&world->floatingMines, sizeof(t_floatingMine), 4);
	dynamicArray_Init(&world->magneticMines, sizeof(t_magneticMine), 4);
	dynamicArray_Init(&world->spawners, sizeof(t_magneticMine), 4);

	world->isPaused = false;

	newWorldLevel(world, 1);

    return world;
}

void world_destroy(t_world* world)
{
	dynamicArray_Destroy(&world->floatingMines);
	dynamicArray_Destroy(&world->magneticMines);
	//dynamicArray_Destroy(&world->fireballMines);

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		player_destroy(dynamicArray_GetItem(&world->players, i));
	}
	dynamicArray_Destroy(&world->spawners);
	dynamicArray_Destroy(&world->players);

    free(world);
}

void newWorldLevel(t_world* world, unsigned int level)
{
	//set spawners
	unsigned int nbSpawners = pow((level % 3), 1.1) + 2 + (level / 3);

	vector2D* spawner;

	for (unsigned int i = 0; i < nbSpawners; i++)
	{
		spawner = dynamicArray_AddItem(&world->spawners);
		*spawner = getRandomLocation();
	}
}

void world_render(t_world* world, t_assets* assets)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->floatingMines, &i); i++)
	{
		t_floatingMine* floatingMine = dynamicArray_GetItem(&world->floatingMines, i);
		floatingMine_render(floatingMine, assets->render);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->magneticMines, &i); i++)
	{
		t_magneticMine* magneticMine = dynamicArray_GetItem(&world->magneticMines, i);
		magneticMine_render(magneticMine, assets->render);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		player_render(player, assets->render);
	}
}

void world_tick(t_world* world, float deltaTime)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		player_tick(dynamicArray_GetItem(&world->players, i), deltaTime);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->floatingMines, &i); i++)
	{
		t_floatingMine* floatingMine = dynamicArray_GetItem(&world->floatingMines, i);
		floatingMine_tick(floatingMine, deltaTime);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->magneticMines, &i); i++)
	{
		t_magneticMine* magneticMine = dynamicArray_GetItem(&world->magneticMines, i);
		magneticMine_tick(magneticMine, deltaTime);
		magneticMine->target = dynamicArray_GetItem(&world->players, 0);
	}
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

void world_collisions(t_world* world)
{
	//for each player
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		
		for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->floatingMines, &j); j++)
		{
			t_entity* entity = &((t_floatingMine*) dynamicArray_GetItem(&world->floatingMines, j))->entity; //&world->floatingMines[j]->entity;
			if (polygon_polgyon_collision(&player->entity.worldCollider, &entity->worldCollider))
			{
				//fprintf(stderr, "OOH WILFRE!\n");
			}
			else
			{
				//fprintf(stderr, "no collision !\n");
			}

			for (unsigned int k = 0; dynamicArray_GetValidItemIndex(&player->bullets, &k); k++)
			{
				t_bullet* bullet = dynamicArray_GetItem(&player->bullets, k);
				if (circle_polgyon_collision(&bullet->collision, &entity->worldCollider))
				{
					bullet_destroy(bullet);
					dynamicArray_RemoveItem(&player->bullets, k);
					dynamicArray_RemoveItem(&world->floatingMines, j);

					player->score += 100;
				}
			}
		}
	}
}

bool world_pause(t_world* world, t_assets* assets)
{
	if (!world->isPaused)
		return false;

	return true;
}

void world_loop(t_assets* assets, float deltaTime, game* game)
{
    t_world* world = ((t_world*) (game->level.data));

	if (world->spawners.usedItems > 0)
	{
		unsigned int i = 0;
		dynamicArray_GetValidItemIndex(&world->spawners, &i);
		vector2D* spawner = dynamicArray_GetItem(&world->spawners, i);

		if (world->spawners.usedItems < 3)
		{
			t_floatingMine* mine = dynamicArray_AddItem(&world->floatingMines);
			mine_spawn(mine, 0, *spawner);
		}
		else
		{
			t_floatingMine* mine = dynamicArray_AddItem(&world->magneticMines);
			mine_spawn(mine, 1, *spawner);
		}
		
		dynamicArray_RemoveItem(&world->spawners, i);
	 }

	world_render(world, assets);

	world_inputs(game, world);

	if (world_pause(world, assets))
		return;

	world_tick(world, deltaTime);

	world_collisions(world);
}