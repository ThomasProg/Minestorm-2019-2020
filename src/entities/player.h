#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>

#include "entities/entities.h"

#include "bullets.h"
#include "dynamicArray/dynamicArray.h"

typedef struct 
{
	bool isLeft;
	bool isRight;
	bool isForward;
	bool shoot;
	bool teleport;
} bInputs;

typedef struct 
{
	//move keys
	int forward;
	int left;
	int right;

	//action keys
	int shoot;
	int teleport1;
	int teleport2;
} inputValues;

typedef struct s_player
{
	unsigned int life; 
	unsigned int score;

    t_entity entity;

	//t_dynamicArray bullets;
	unsigned int lastShoot; //in milliseconds

	bInputs bInputs;
	inputValues inputValues;
} t_player;

void player_init(t_player* player);
void player_destroy(t_player* player);
void player_render(t_player* player, t_render* render, bool renderDebug);
void player_tick(t_player* player, float deltaTime);

bInputs bInput_create();
void player_inputs_run(t_player* player, float deltaTime);
void player_input_start(t_player* player, int key, bool start);

inputValues getInputValues(unsigned int id);
inputValues getInputValues1();
inputValues getInputValues2();

void player_damages(t_player* player, t_dynamicArray* players, unsigned int id);

#endif