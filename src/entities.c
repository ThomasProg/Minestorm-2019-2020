#include "entities.h"
#include <stdlib.h>

inputs input_create()
{
	inputs inputs;
	inputs.isForward = false;
	inputs.isLeft = false;
	inputs.isRight = false;

	return inputs;
}

t_entity* entity_create()
{
	t_entity* entity = malloc(sizeof(*entity));
	// stat_init(&entity->life, 0, 20, 20);
	// stat_init(&entity->speed, 0, 50, 100); 
	// stat_init(&entity->power, 0, 20, 20);

	// vector2D* location = &entity->physics.location;
	// vector2D* speed = &entity->physics.speed;
	// vector2D* acceleration = &entity->physics.acceleration;

	// location->x = 500;
	// location->y = 500;
	// entity->ref.origin = (vector2D) {location->x , location->y};
	// entity->ref.unitI = (vector2D) {1.0 , 0.0};
	// entity->ref.unitJ = (vector2D) {0.0 , 1.0};

	// speed->x = 0;
	// speed->y = 0;

	// acceleration->x = 0;
	// acceleration->y = 0;
	vector2D* points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, 50.0};
	points[2] = (point2D) {100, 0.0};
	entity->collision.leftSide.points = points;
	entity->collision.leftSide.size = 3;

	points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, -50.0};
	points[2] = (point2D) {100.0, 0.0};
	entity->collision.rightSide.points = points;
	entity->collision.rightSide.size = 3;

	entity->ref.unitI = (vector2D) {1, 0};
	entity->ref.unitJ = (vector2D) {0, 1};
	entity->ref.origin = (vector2D) {300, 300};
	entity->life = 0;
	entity->score = 0;

	entity->speed = nullVector();
	entity->acceleration = nullVector();

	entity->inputs = input_create();

	return entity;
}

void collisions_render(SDL_Renderer* renderer, convexPolygon polygon, referential* ref)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0 ,255);

	for (unsigned i = 0; i < polygon.size; i++)
	{
		unsigned int j = (i + 1) % polygon.size;
		//vector2D vec = substractVectors(polygon.points[i], polygon.points[j]);

		//point2D a = addVectors(scaleVector(ref->unitI, polygon.points[i].x), 
								//scaleVector(ref->unitJ, polygon.points[j].y));
		//point2D b = {0, 0};

		point2D a = localToWorld_point2D(polygon.points[i], *ref);
		point2D b = localToWorld_point2D(polygon.points[j], *ref);

		SDL_RenderDrawLine(renderer,
                a.x + ref->origin.x,
                a.y + ref->origin.y,
                b.x + ref->origin.x,
				b.y + ref->origin.y);

		// SDL_RenderDrawLine(renderer,
        //                 polygon.points[i].x * ref->unitI + ref->origin.x,
        //                 polygon.points[i].y + ref->origin.y,
        //                 polygon.points[(i + 1) % polygon.size].x + ref->origin.x,
		// 				polygon.points[(i + 1) % polygon.size].y + ref->origin.y);
	}
}

void entity_render(t_entity* entity, t_render* render)
{
	entity->texture = render_get(render, 0);
	
	//SDL_Rect rect = {entity->physics.location.x/10, entity->physics.location.y/10,50,100};
	SDL_Rect rect = {entity->ref.origin.x, entity->ref.origin.y,50,100};
	SDL_Rect src = {0,100,50,100};

	SDL_RenderCopy(render->renderer, entity->texture, &src, &rect);

	collisions_render(render->renderer, entity->collision.leftSide, &entity->ref);
	collisions_render(render->renderer, entity->collision.rightSide, &entity->ref);
}

void entity_tick(t_entity* entity)
{
	entity_inputs_run(entity);

	vector2D *speed = &entity->speed, 
		*acceleration = &entity->acceleration,
		*location = &entity->ref.origin;

	float friction = 0.1;

	//air friction
	if (speed->x > 0)
		speed->x-= friction;
	if (speed->x < 0)
		speed->x += friction;
	if (speed->y > 0)
		speed->y -= friction;
	if (speed->y < 0)
		speed->y += friction;
	
	speed->x += acceleration->x;
	speed->y += acceleration->y;

	location->x += speed->x;
	location->y += speed->y;
}

void entity_inputs_run(t_entity* entity)
{
	float currentSpeed = 5.0;

	vector2D *speed = &entity->speed;

	vector2D newSpeed;
	decimal angle;

	if (entity->inputs.isForward)
	{
		newSpeed.x = 0;
		newSpeed.y = 1.0 * currentSpeed;
		angle = - vectorAngle(entity->ref.unitI);
		newSpeed = rotateVector(newSpeed, angle);

		*speed = addVectors(*speed, newSpeed);
		if (speed->x > 4)
			speed->x = 4;
		if (speed->x < -4)
			speed->x = -4;
		if (speed->y > 4)
			speed->y = 4;
		if (speed->y < -4)
			speed->y = -4;
	}

	if (entity->inputs.isLeft)
	{
		entity->ref.unitI = rotateVector(entity->ref.unitI, - 0.1);
		entity->ref.unitJ = rotateVector90(entity->ref.unitI);
	}

	if (entity->inputs.isRight)
	{
		entity->ref.unitI = rotateVector(entity->ref.unitI, 0.1);
		entity->ref.unitJ = rotateVector90(entity->ref.unitI);
	}
}

void entity_input_start(t_entity* entity, int key, bool start)
{
	switch (key)
	{
		case SDLK_w :
			entity->inputs.isForward = start;
			break;
		
		case SDLK_a :
			entity->inputs.isLeft = start;
			break;
		case SDLK_d :
			entity->inputs.isRight = start;
			break;
		
	}
}
