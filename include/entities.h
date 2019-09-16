#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "stats.h"
#include "vector2D/objects.h"
#include "SDL_utilities/render.h"
#include "physics.h"

#include <SDL2/SDL.h>

typedef struct s_entity
{
	t_stat life;
	t_stat speed;
	t_stat power;

	physics physics;
	referential ref;
	//vector2D location, speed, acceleration;
	//int x, y;
	//int vx, vy;
	//int ax, ay;
	
	
	SDL_Texture* texture;
} t_entity;

t_entity* entity_create();

void entity_render(t_entity* entity, t_render* render);

void entity_tick(t_entity* entity);

void entity_input(t_entity* entity, int key);

#endif
