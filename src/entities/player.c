#include <time.h>

#include "player.h"
#include "macros.h"

void player_collision_init(playerCollision* collision)
{
	vector2D* points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, 50.0};
	points[2] = (point2D) {100, 0.0};
	collision->leftSide.points = points;
	collision->leftSide.size = 3;

	points = malloc(3 * sizeof(point2D));
	points[0] = (point2D) {0.0, 0.0};
	points[1] = (point2D) {-50.0, -50.0};
	points[2] = (point2D) {100.0, 0.0};
	collision->rightSide.points = points;
	collision->rightSide.size = 3;
}

inputs input_create()
{
	inputs inputs;
	inputs.isForward = false;
	inputs.isLeft = false;
	inputs.isRight = false;

	return inputs;
}

t_player* player_create()
{
	t_player* player = malloc(sizeof(*player));

    entity_init(&player->entity);

    player->entity.collision = malloc(sizeof(playerCollision));
    player_collision_init(player->entity.collision);

	player->life = 0;
	player->score = 0;

	player->inputs = input_create();

	return player;
}

void player_render(t_player* player, t_render* render)
{
    entity_render(&player->entity, render);

    playerCollision* collision = player->entity.collision;
    collisions_render(render->renderer, collision->leftSide, &player->entity.ref);
	collisions_render(render->renderer, collision->rightSide, &player->entity.ref);
    
    //player->entity.aabb = aabbRectangleGenerate(collision->leftSide.points, 4);
    axisAlignedRectangle rec = aabbRectangleGenerate(collision->leftSide.points, 4);
    rec.center = addVectors(rec.center, player->entity.ref.origin); 
    axisAlignedRectangle_render(render->renderer, player->entity.aabb);
}

void player_tick(t_player* player)
{
    player_inputs_run(player);
    entity_tick(&player->entity);
}

void player_inputs_run(t_player* player)
{
	vector2D *speed = &player->entity.speed;

	vector2D newSpeed;
	decimal angle;

	if (player->inputs.isForward)
	{
		newSpeed.x = 0;
		newSpeed.y = SPEED;
		angle = - vectorAngle(player->entity.ref.unitI);
		newSpeed = rotateVector(newSpeed, angle);

		*speed = addVectors(*speed, newSpeed);

        float totalSpeed = vectorLength(*speed);

        if (totalSpeed > SPEED_LIMIT)
        {
            *speed = scaleVector(unitVector(*speed), SPEED_LIMIT);
        }

	}

	if (player->inputs.isLeft)
	{
		player->entity.ref.unitI = rotateVector(player->entity.ref.unitI, - ROTATION);
		player->entity.ref.unitJ = rotateVector90(player->entity.ref.unitI);
	}

	if (player->inputs.isRight)
	{
		player->entity.ref.unitI = rotateVector(player->entity.ref.unitI, ROTATION);
		player->entity.ref.unitJ = rotateVector90(player->entity.ref.unitI);
	}
}

int random_id_generator = 0;

void player_input_start(t_player* player, int key, bool start)
{
	vector2D* loc = &player->entity.ref.origin;
	srand(time(NULL) + random_id_generator);   // Initialization, should only be called once.
    random_id_generator++;
	float rx = rand() % WINDOW_SIZE_X;    
	float ry = rand() % WINDOW_SIZE_Y;     

	switch (key)
	{
		case SDLK_r :
			player->inputs.isForward = start;
			break;
		
		case SDLK_d :
			player->inputs.isLeft = start;
			break;
		case SDLK_g :
			player->inputs.isRight = start;
			break;
		
		case SDLK_e :
			loc->x = rx;
			loc->y = ry;
		case SDLK_t :
			loc->x = rx;
			loc->y = ry;
	}
}
