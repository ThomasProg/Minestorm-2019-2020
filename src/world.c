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

point2D* spawners_generate(unsigned int nb)
{
	point2D* spawners = malloc(nb * sizeof(vector2D));
	for (unsigned int i = 0; i < nb; i++)
	{
		spawners[i] = getRandomLocation();
	}
	return spawners;
}

t_player** players_generate()
{
    t_player** players = malloc(2 * sizeof(t_player*));
    
    players[0] = player_create();
	players[0]->inputValues = getInputValues1();

    players[1] = player_create();
	players[1]->inputValues = getInputValues2();

    return players;
}

t_world* world_create()
{
    t_world* world = malloc(sizeof(t_world));
	dynamicArray_Init(&world->floatingMines, sizeof(t_floatingMine));
	dynamicArray_Init(&world->magneticMines, sizeof(t_magneticMine));
	//dynamicArray_Init(&world->fireballMines, sizeof(t_fireballMine));
	//world->nbfloatingMines = 0;
	//world->floatingMines = malloc(50 * sizeof(t_floatingMine*));
    //entitiesList_init(world->entities);
    world->players = players_generate();
	point2D* spawners_generate(unsigned int nb);

	newWorldLevel(world, 1);

    return world;
}

void newWorldLevel(t_world* world, unsigned int level)
{
	unsigned int nbSpawners = pow((level % 3), 1.1) + 2 + (level / 3);
	world->nbSpawners = nbSpawners;

	world->spawners = spawners_generate(nbSpawners);
}

void world_loop(t_assets* assets, float deltaTime, game* game)
{
    t_world* world = ((t_world*) (game->level.data));

	player_tick(world->players[0], deltaTime);
	player_tick(world->players[1], deltaTime);

    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN) //press
		{
			int key = event.key.keysym.sym;
			if (key == SDLK_ESCAPE)
				game->run = false;
			player_input_start(world->players[0], key, true);
		    player_input_start(world->players[1], key, true);
		}
		if (event.type == SDL_KEYUP) //release
		{
			int key = event.key.keysym.sym;
			player_input_start(world->players[0], key, false);
			player_input_start(world->players[1], key, false);
		}
		if (event.type == SDL_QUIT)
			game->run = false;
	}

	 if (world->nbSpawners > 0)
	 {
	 	world->nbSpawners--;
		if (world->nbSpawners < 2)
		{
			t_floatingMine* mine = dynamicArray_AddItem(&world->floatingMines);
			mine_spawn(mine, 0, world->spawners, world->nbSpawners);
		}
		else
		{
			t_floatingMine* mine = dynamicArray_AddItem(&world->magneticMines);
			mine_spawn(mine, 1, world->spawners, world->nbSpawners);
		}
		
	 }

	for (unsigned int i = 0; dynamicArray_ForEachUsed(&world->floatingMines, &i); i++)
	{
		t_floatingMine* floatingMine = dynamicArray_GetItem(&world->floatingMines, i);
		floatingMine_render(floatingMine, assets->render);
		floatingMine_tick(floatingMine, deltaTime);
	}

	for (unsigned int i = 0; dynamicArray_ForEachUsed(&world->magneticMines, &i); i++)
	{
		t_magneticMine* magneticMine = dynamicArray_GetItem(&world->magneticMines, i);
		magneticMine_render(magneticMine, assets->render);
		magneticMine_tick(magneticMine, deltaTime);
		magneticMine->target = world->players[0];
	}

	//for each player
	for (unsigned int i = 0; i < 2; i++)
	{
		t_player* player = world->players[i];
		player_render(player, assets->render);
		
		for (unsigned int j = 0; dynamicArray_ForEachUsed(&world->floatingMines, &j); j++)
		{
			t_entity* entity = &((t_floatingMine*) dynamicArray_GetItem(&world->floatingMines, j))->entity; //&world->floatingMines[j]->entity;
			if (polygon_polgyon_collision(&player->entity.worldCollider, &entity->worldCollider))
			{
				fprintf(stderr, "OOH WILFRE!\n");
			}
			else
			{
				fprintf(stderr, "no collision !\n");
			}

			for (unsigned int k = 0; k < player->nbBullets; k++)
			{
				t_bullet* bullet = player->bullets[k];
				if (bullet->isAlive)
				{
					if (circle_polgyon_collision(&player->bullets[k]->collision, &entity->worldCollider))
					{
						dynamicArray_RemoveItem(&world->floatingMines, j);
						bullet->isAlive = false;
						world->players[0]->score += 100;
					}
				}
			}
		}

	}
}

void world_destroy(t_world* world)
{
	dynamicArray_Destroy(&world->floatingMines);
	dynamicArray_Destroy(&world->magneticMines);
	//dynamicArray_Destroy(&world->fireballMines);

    free(world);
}

