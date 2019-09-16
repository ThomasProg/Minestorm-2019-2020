#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include <entities/entities.h>

typedef struct 
{
	convexPolygon rightSide;
	convexPolygon leftSide;
} playerCollision;

typedef struct 
{
	bool isLeft;
	bool isRight;
	bool isForward;
} inputs;

typedef struct s_player
{
	unsigned int life; 
	unsigned int score;

    t_entity entity;

	inputs inputs;
} t_player;

t_player* player_create();
void player_render(t_player* player, t_render* render);
void player_tick(t_player* player);

inputs input_create();
void player_inputs_run(t_player* player);
void player_input_start(t_player* player, int key, bool start);

#endif