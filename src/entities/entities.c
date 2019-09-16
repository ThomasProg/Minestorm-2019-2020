#include "entities.h"

#include <time.h>
#include "macros.h"
#include <stdlib.h>

t_entity* entity_create()
{
	t_entity* entity = malloc(sizeof(*entity));
	return entity;
}

void entity_init(t_entity* entity)
{
	entity->ref.unitI = (vector2D) {1, 0};
	entity->ref.unitJ = (vector2D) {0, 1};
	entity->ref.origin = (vector2D) {300, 300};

	entity->speed = nullVector();
	entity->acceleration = nullVector();
}

void axisAlignedRectangle_render(SDL_Renderer* renderer, axisAlignedRectangle rectangle)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0 ,255);

	point2D points[4];
	points[0] = (point2D) {rectangle.halfSize.x, rectangle.halfSize.y};
	points[1] = (point2D) {rectangle.halfSize.x, -rectangle.halfSize.y};
	points[2] = (point2D) {-rectangle.halfSize.x, -rectangle.halfSize.y};
	points[3] = (point2D) {-rectangle.halfSize.x, rectangle.halfSize.y};

	for (int i = 0; i < 4; i++)
		points[i] = addVectors(points[i], rectangle.center);

	for (unsigned i = 0; i < 4; i++)
	{
		unsigned int j = (i + 1) % 4;

		point2D a = points[i];
		point2D b = points[j];

		SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
	}
}

void collisions_render(SDL_Renderer* renderer, convexPolygon polygon, referential* ref)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0 ,255);

	for (unsigned i = 0; i < polygon.size; i++)
	{
		unsigned int j = (i + 1) % polygon.size;

		point2D a = localToWorld_point2D(polygon.points[i], *ref);
		point2D b = localToWorld_point2D(polygon.points[j], *ref);

		SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
	}
}

void entity_render(t_entity* entity, t_render* render)
{
	entity->texture = render_get(render, 0);
	
	//SDL_Rect rect = {entity->ref.origin.x, entity->ref.origin.y,50,100};
	//SDL_Rect src = {0,100,50,100};

	//SDL_RenderCopy(render->renderer, entity->texture, &src, &rect);

	// collisions_render(render->renderer, entity->collision.leftSide, &entity->ref);
	// collisions_render(render->renderer, entity->collision.rightSide, &entity->ref);
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

void entity_tick(t_entity* entity)
{
	vector2D *speed = &entity->speed, 
		*acceleration = &entity->acceleration,
		*location = &entity->ref.origin;

	float totalSpeed = vectorLength(*speed);
	if (totalSpeed > 0)
	{
		if (totalSpeed < AIR_FRICTION)
			*speed = nullVector();
		else
 	   		*speed = scaleVector(unitVector(*speed), totalSpeed - AIR_FRICTION);
	}

	speed->x += acceleration->x;
	speed->y += acceleration->y;

	location->x += speed->x;
	location->y += speed->y;
}