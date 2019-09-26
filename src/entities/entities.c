#include "entities.h"
#include "player.h"
#include "worldField.h"
#include "macros.h"

#include <time.h>
#include <stdlib.h>

t_entity* entity_create()
{
	t_entity *entity = malloc(sizeof(*entity));
	return entity;
}

void entity_init(t_entity* entity)
{
	entity->ref.unitI = (vector2D){1.f, 0.f};
	entity->ref.unitJ = (vector2D){0.f, 1.f};
	entity->ref.origin = (vector2D){300, 300};

	entity->maxSpeed = 500.f;
	entity->isTeleportingAtBorder = true;

	entity->worldCollider.convexPolygons = NULL;

	entity->velocity = nullVector();
}

void entity_destroy(t_entity* entity)
{
	//cast : WARNING
	polygon* a = entity->collision;

	free(a->convexPolygons);

	free(a);

	if (entity->texture != NULL)
		free(entity->texture);
}

void entity_render(t_entity* entity, t_render* render, bool renderDebug)
{
	if (entity->worldCollider.convexPolygons != NULL)
		polygon_free(&entity->worldCollider);

	polygon* poly = entity->collision;
	entity->worldCollider = localToWorld_polygon(poly, entity->ref);

	//render referential
	if (renderDebug)
	{
		point2D center = entity->ref.origin;
		point2D iAxis = addVectors(center, scaleVector(entity->ref.unitI, 50));
		point2D jAxis = addVectors(center, scaleVector(entity->ref.unitJ, 50));
		SDL_SetRenderDrawColor(render->renderer, 0, 0, 255, 255);
		SDL_RenderDrawLine(render->renderer, center.x, center.y, iAxis.x, iAxis.y);
		SDL_SetRenderDrawColor(render->renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(render->renderer, center.x, center.y, jAxis.x, jAxis.y);
	}
}

float airFriction(float value, float friction)
{
	if (value > 0.0)
	{
		if (value > friction)
		{
			value -= friction;
		}
		else
		{
			value = 0.0;
		}
	}

	return value;
}

void entity_tick(t_entity* entity, float deltaTime)
{
	vector2D *velocity = &entity->velocity,
			 *location = &entity->ref.origin;

	//air friction
	float totalSpeed = vectorLength(*velocity);
	if (totalSpeed > 0)
	{
		if (totalSpeed < deltaTime * AIR_FRICTION)
			*velocity = nullVector();
		else
			*velocity = scaleVector(unitVector(*velocity), totalSpeed - deltaTime * AIR_FRICTION);
	}

	//add speed to loc
	location->x += velocity->x * deltaTime;
	location->y += velocity->y * deltaTime;

	//DRIFT
	//straighten velocity
	float velocityMagnitude = vectorLength(*velocity);
	if (floatIsNearlyEqual(velocityMagnitude, 0.0, 0.1f)) //useless to straighten if there is no speed
	{
		//since the drift is relative to the player : 
		*velocity = worldToLocal_vector2D(*velocity, entity->ref);
		velocity->x += DRIFT_LEVEL * deltaTime;
		if (vectorLength(*velocity) != 0.f)
			*velocity = scaleVector(unitVector(*velocity), velocityMagnitude);
		*velocity = localToWorld_vector2D(*velocity, entity->ref);
	}

	if (entity->isTeleportingAtBorder)
		border_teleportation(&entity->ref.origin);
}

void entity_moveForward(t_entity* entity, float deltaTime)
{
	vector2D acc; 
	vector2D *velocity = &entity->velocity;

	//set acceleration relative to player
	acc.x = ACCELERATION * deltaTime / ENTITY_MASS;
	acc.y = 0;
	//deltaTime is used because acc is the derivative of speed,
	//which is the derivative to the location

	acc = localToWorld_vector2D(acc, entity->ref);

	//add acceleration to velocity
	*velocity = addVectors(*velocity, acc);

	float totalSpeed = vectorLength(*velocity);

	//limit speed
	if (totalSpeed > entity->maxSpeed)
		*velocity = scaleVector(unitVector(*velocity), entity->maxSpeed);
}

void entity_move(t_entity* entity, E_MOVE move, float deltaTime)
{
	switch (move)
	{
		case E_LEFT:
			entity->ref.unitI = rotateVector(entity->ref.unitI, - ANGULAR_SPEED * deltaTime);
			entity->ref.unitI = unitVector(entity->ref.unitI);
			entity->ref.unitJ = rotateVector90(entity->ref.unitI);
			break;

		case E_RIGHT:
			entity->ref.unitI = rotateVector(entity->ref.unitI, ANGULAR_SPEED * deltaTime);
			entity->ref.unitI = unitVector(entity->ref.unitI);
			entity->ref.unitJ = rotateVector90(entity->ref.unitI);
			break;

		case E_FORWARD:
			entity_moveForward(entity, deltaTime);
			break;
	}
}

void entity_teleport(t_entity* entity)
{
	vector2D* loc = &entity->ref.origin;
	vector2D* velocity = &entity->velocity;
	float rx = rand() % WINDOW_SIZE_X;    
	float ry = rand() % WINDOW_SIZE_Y;  

	loc->x = rx;
	loc->y = ry;
	velocity->x = 0.f;
	velocity->y = 0.f;
}