#include "worldField.h"
#include "macros.h"

void border_teleportation(vector2D* location)
{
	//teleport if is out of border
	if (location->x < 0.0)
		location->x += WINDOW_SIZE_X;

	if (location->x > WINDOW_SIZE_X)
		location->x -= WINDOW_SIZE_X;

	if (location->y < 0.0)
		location->y += WINDOW_SIZE_Y;

	if (location->y > WINDOW_SIZE_Y)
		location->y -= WINDOW_SIZE_Y;
}