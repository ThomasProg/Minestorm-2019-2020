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

void entity_render(t_entity* entity, t_render* render)
{
	if (entity->worldCollider.convexPolygons != NULL)
	{
		for (unsigned int i = 0; i < entity->worldCollider.nbConvexPolygons; i++)
		{
			free(entity->worldCollider.convexPolygons[i].points);
		}
		free(entity->worldCollider.convexPolygons);
	}

	polygon* poly = entity->collision;
	entity->worldCollider = localToWorld_polygon(poly, entity->ref);
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

	border_teleportation(&entity->ref.origin);

	//straighten velocity
	float velocityMagnitude = vectorLength(*velocity);
	if (velocityMagnitude != 0.f)
	{
		*velocity = worldToLocal_vector2D(*velocity, entity->ref);
		velocity->x += DRIFT_LEVEL * deltaTime;
		if (vectorLength(*velocity) != 0.f)
			*velocity = scaleVector(unitVector(*velocity), velocityMagnitude);
		*velocity = localToWorld_vector2D(*velocity, entity->ref);
	}
}

void entity_move(t_entity* entity, E_MOVE move, float deltaTick)
{
	vector2D acc; //acceleration
	decimal angle;

	vector2D *velocity = &entity->velocity;

	switch (move)
	{
		case E_LEFT:
			entity->ref.unitI = rotateVector(entity->ref.unitI, - ANGULAR_SPEED * deltaTick);
			entity->ref.unitI = unitVector(entity->ref.unitI);
			entity->ref.unitJ = rotateVector90(entity->ref.unitI);
			break;

		case E_RIGHT:
			entity->ref.unitI = rotateVector(entity->ref.unitI, ANGULAR_SPEED * deltaTick);
			entity->ref.unitI = unitVector(entity->ref.unitI);
			entity->ref.unitJ = rotateVector90(entity->ref.unitI);
			break;

		case E_FORWARD:
			acc.x = 0;
			acc.y = ACCELERATION * deltaTick / ENTITY_MASS;
			angle = -vectorAngle(entity->ref.unitI);
			acc = rotateVector(acc, angle);

			*velocity = addVectors(*velocity, acc);

			float totalSpeed = vectorLength(*velocity);

			if (totalSpeed > entity->maxSpeed)
			{
				*velocity = scaleVector(unitVector(*velocity), entity->maxSpeed);
			}
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