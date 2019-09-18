#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "stats.h"
#include "vector2D/objects.h"
#include "SDL_utilities/render.h"
#include "physics.h"
#include "renderShapes.h"

#include <SDL2/SDL.h> 

typedef enum 
{
	E_PLAYER = 0,
	E_MINE = 1,
	E_FLOATING_MINE = 2,
	E_MAGNETIC_MINE
} E_COLLISION_TYPE;

typedef enum 
{
	E_RIGHT = 0,
	E_LEFT = 1,
	E_FORWARD = 2,
} E_MOVE;

typedef struct s_entity
{
	vector2D velocity;
	referential ref;

	axisAlignedRectangle aabb;
	void* collision;
	E_COLLISION_TYPE collisionType;
	
	SDL_Texture* texture;
} t_entity;

t_entity* entity_create();
void entity_init(t_entity* entity);

void entity_render(t_entity* entity, t_render* render);

void entity_tick(t_entity* entity, float deltaTime);
void entity_border_teleportation(t_entity* entity);

void entity_move(t_entity* entity, E_MOVE move, float deltaTick);
void entity_teleport(t_entity* entity);

#endif
