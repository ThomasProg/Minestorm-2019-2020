#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entities.h"

typedef struct s_player
{
	t_entity entity;

} t_player;

t_player* player_create();

//void player_render(t_player* player);

#endif
