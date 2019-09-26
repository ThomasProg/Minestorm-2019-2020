#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "stats.h"
#include "vector2D/referential.h"
#include "SDL_utilities/render.h"
#include "renderShapes.h"

#include <SDL2/SDL.h> 

//used to switch between collision types;
//however, they are all the same for the moment (polygon type),
//so it would be useful for future development
typedef enum 
{
	E_PLAYER = 0,
	E_MINE = 1,
} E_COLLISION_TYPE;

typedef enum 
{
	E_RIGHT = 0,
	E_LEFT = 1,
	E_FORWARD = 2,
} E_MOVE;

typedef struct s_entity
{
	bool isTeleportingAtBorder;
	float maxSpeed;

	vector2D velocity;
	referential ref;

	void* collision;
	polygon worldCollider;
	E_COLLISION_TYPE collisionType;
	
	SDL_Texture* texture; 
} t_entity;

t_entity* entity_create();
void entity_init(t_entity* entity);

void entity_render(t_entity* entity, t_render* render, bool renderDebug);

void entity_tick(t_entity* entity, float deltaTime);

void entity_move(t_entity* entity, E_MOVE move, float deltaTime);
void entity_teleport(t_entity* entity);

#endif
