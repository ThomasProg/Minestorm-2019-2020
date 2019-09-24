#include <stdbool.h>
#include "renderShapes.h"
#include "vector2D/collisions2D.h"

void convexPolygon_render(SDL_Renderer* renderer, convexPolygon convex, bool renderAABB)
{
	for (unsigned i = 0; i < convex.size; i++)
	{
		unsigned int j = (i + 1) % convex.size;

		point2D a = convex.points[i];
		point2D b = convex.points[j];

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
		
		if (renderAABB)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			axisAlignedRectangle_render(renderer, convex.aabb);
		}
	}
}

void polygon_render(SDL_Renderer* renderer, polygon* polygon, bool renderAABB)
{
    for (unsigned int i = 0; i < polygon->nbConvexPolygons; i++)
    {
        convexPolygon_render(renderer, polygon->convexPolygons[i], renderAABB);
		
		if (renderAABB)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			axisAlignedRectangle_render(renderer, polygon->aabb);
		}
    }
}

void axisAlignedRectangle_render(SDL_Renderer* renderer, axisAlignedRectangle rectangle)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	point2D points[4];
	points[0] = (point2D){rectangle.halfSize.x, rectangle.halfSize.y};
	points[1] = (point2D){rectangle.halfSize.x, -rectangle.halfSize.y};
	points[2] = (point2D){-rectangle.halfSize.x, -rectangle.halfSize.y};
	points[3] = (point2D){-rectangle.halfSize.x, rectangle.halfSize.y};

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

