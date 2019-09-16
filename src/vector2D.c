#include "vector2D.h"
#include <stdio.h>

int vector2D_equal(t_vector2D* a, t_vector2D* b)
{
	int x = a->x - b->x;
	if (x < 0)
		x *= -1;
	int y = a->y - b->y;
	if (y < 0)
		y *= -1;
	return x < 1 && y < 1;
}
