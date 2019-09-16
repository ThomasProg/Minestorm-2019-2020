#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "stats.h"
#include "vector2D/objects.h"
#include "SDL_utilities/render.h"
#include "physics.h"

#include <SDL2/SDL.h> 

typedef enum 
{
	E_PLAYER = 0,
	E_MINE = 1
} E_COLLISION_TYPE;

typedef struct s_entity
{
	vector2D speed, acceleration;
	referential ref;

	axisAlignedRectangle aabb;
	void* collision;
	E_COLLISION_TYPE collisionType; //TODO : enum
	
	SDL_Texture* texture;
} t_entity;

void axisAlignedRectangle_render(SDL_Renderer* renderer, axisAlignedRectangle rectangle);
void collisions_render(SDL_Renderer* renderer, convexPolygon polygon, referential* ref);

t_entity* entity_create();
void entity_init(t_entity* entity);

void entity_render(t_entity* entity, t_render* render);

void entity_tick(t_entity* entity);

#endif
