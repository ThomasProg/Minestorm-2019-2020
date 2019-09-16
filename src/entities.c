#include "entities.h"
#include <stdlib.h>

t_entity* entity_create()
{
	t_entity* entity = malloc(sizeof(*entity));
	stat_init(&entity->life, 0, 20, 20);
	stat_init(&entity->speed, 0, 50, 100);
	stat_init(&entity->power, 0, 20, 20);

	vector2D* location = &entity->physics.location;
	vector2D* speed = &entity->physics.speed;
	vector2D* acceleration = &entity->physics.acceleration;

	location->x = 500;
	location->y = 500;
	entity->ref.origin = (vector2D) {location->x , location->y};
	entity->ref.unitI = (vector2D) {1.0 , 0.0};
	entity->ref.unitJ = (vector2D) {0.0 , 1.0};

	speed->x = 0;
	speed->y = 0;

	acceleration->x = 0;
	acceleration->y = 0;

	return entity;
}

void entity_render(t_entity* entity, t_render* render)
{
	entity->texture = render_get(render, 0);
	
	SDL_Rect rect = {entity->physics.location.x/10, entity->physics.location.y/10,50,100};
	SDL_Rect src = {0,100,50,100};

	SDL_RenderCopy(render->renderer, entity->texture, &src, &rect);
}

void entity_tick(t_entity* entity)
{
	vector2D* location = &entity->physics.location;
	vector2D* speed = &entity->physics.speed;
	vector2D* acceleration = &entity->physics.acceleration;
	
	//fprintf(stderr, "%f, %f\n", speed->x, speed->y);

	//acceleration->y += 1; //gravity force

	//air friction
	if (speed->x > 0)
		speed->x-= 1;
	if (speed->x < 0)
		speed->x += 1;
	if (speed->y > 0)
		speed->y -= 1;
	if (speed->y < 0)
		speed->y += 1;
	
	speed->x += acceleration->x;
	speed->y += acceleration->y;
	
	if (location->y/10 > 300)
		if (speed->y > 0)
		{
			acceleration->y = 0;
			speed->y = 0;
		}

	if (location->x/10 < 0)
		if (speed->x < 0)
		{
			acceleration->x = 0;
			speed->x = 0;
		}

	*location = addVectors(localToWorld_vector2D(*speed, entity->ref), *location);
	//location->x += speed->x;
	//location->y += speed->y;
	entity->ref.origin = (vector2D) {location->x , location->y};
}

void entity_input(t_entity* entity, int key)
{
	unsigned int current_speed = entity->speed.current;
	//current_speed = current_speed;
	//vector2D* location = &entity->physics.location;
	vector2D* speed = &entity->physics.speed;
	vector2D* acceleration = &entity->physics.acceleration;
	
	switch (key)
	{
		case SDLK_o :
			speed->y = - 200;
			acceleration->y = 0;
			break;
		
		case SDLK_w :
			speed->y = - 1.0 * current_speed;
			break;
		case SDLK_s :
			speed->y = + 1.0 * current_speed;
			break;
		
		case SDLK_a :
			speed->x = - 1.0 * current_speed;
			//entity->ref.unitI = rotateVector(entity->ref.unitI, 1);
			//entity->ref.unitJ = rotateVector(entity->ref.unitJ, 1);
			break;
		case SDLK_d :
			speed->x = + 1.0 * current_speed;
			break;
		
	}
	
	
}
