#include "world.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"
#include "entities/bullets.h"
#include "vector2D/collisions2D.h"

void entitiesList_init()
{
    // //t_entity** entities;

	// t_floatingMine* floatingMine = floatingMine_create();
	// t_magneticMine* magneticMine = magneticMine_create();
	// t_fireballMine* fireballMine = fireballMine_create();
	// //t_bullet** bullets = malloc(sizeof(t_bullet));
}

t_player** players_init()
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
    //entitiesList_init(world->entities);
    world->players = players_init();

    return world;
}

void world_loop(t_assets* assets, float deltaTime, t_world* world)
{
    // t_entity** entities;
    // t_player** players;
    // deltaTime = deltaTime;

	// assets=assets;
}

void world_destroy(t_world* world)
{
    free(world);
}