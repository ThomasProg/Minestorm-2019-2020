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
	world->nbfloatingMines = 0;
	world->floatingMines = malloc(50 * sizeof(t_floatingMine*));
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
	 	world->floatingMines[world->nbSpawners] = (t_floatingMine*) mine_spawn(world->spawners, world->nbSpawners, 0);
	 	world->nbfloatingMines++;
	 }
	assert(true); //SEARCH AROUND HERE THE SEG FAULT; up or down
	for (unsigned int i = 0; i < world->nbfloatingMines; i++)
		floatingMine_render(world->floatingMines[i], assets->render);

	player_render(world->players[0], assets->render);
	player_render(world->players[1], assets->render);
}

void world_destroy(t_world* world)
{
    free(world);
}