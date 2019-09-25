#include "worldField.h"
#include "macros.h"
#include "time.h"
#include <stdlib.h>

bool border_teleportation(vector2D* location)
{
	//teleport if is out of border
	bool tp = false;
	if (location->x < 0.0)
	{
		tp = true;
		location->x += WINDOW_SIZE_X;
	}

	if (location->x > WINDOW_SIZE_X)
	{
		tp = true;
		location->x -= WINDOW_SIZE_X;
	}

	if (location->y < 0.0)
	{
		tp = true;
		location->y += WINDOW_SIZE_Y;
	}

	if (location->y > WINDOW_SIZE_Y)
	{
		tp = true;
		location->y -= WINDOW_SIZE_Y;
	}
	return tp;
}

vector2D getRandomLocation()
{
	vector2D vec;
	vec.x = rand() % WINDOW_SIZE_X;
	vec.y = rand() % WINDOW_SIZE_Y;
	return vec;
}