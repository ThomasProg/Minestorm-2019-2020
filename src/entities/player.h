#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include <entities/entities.h>

#include "bullets.h"

typedef struct 
{
	bool isLeft;
	bool isRight;
	bool isForward;
} inputs;

typedef struct s_player
{
	bool bullet;
	unsigned int life; 
	unsigned int score;

    t_entity entity;

	inputs inputs;
} t_player;

t_player* player_create();
void player_render(t_player* player, t_render* render);
void player_tick(t_player* player, float deltaTime);
void player_event(t_player* player, t_bullet** bullets);

inputs input_create();
void player_inputs_run(t_player* player, float deltaTime);
void player_input_start(t_player* player, int key, bool start);

#endif