#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>

#include "entities/entities.h"

#include "bullets.h"

typedef struct 
{
	bool isLeft;
	bool isRight;
	bool isForward;
	bool shoot;
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

	t_bullet** bullets; //array of references
	unsigned int nbBullets;

	bInputs bInputs;
	inputValues inputValues;
} t_player;

t_player* player_create();
void player_render(t_player* player, t_render* render);
void player_tick(t_player* player, float deltaTime);
void player_event(t_player* player, t_bullet** bullets);

bInputs bInput_create();
void player_inputs_run(t_player* player, float deltaTime);
void player_input_start(t_player* player, int key, bool start);

inputValues getInputValues1();
inputValues getInputValues2();

#endif