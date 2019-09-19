#include "renderShapes.h"
#include "vector2D/collisions2D.h"

void convexPolygon_render(SDL_Renderer* renderer, convexPolygon polygon, referential* ref)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

	for (unsigned i = 0; i < polygon.size; i++)
	{
		unsigned int j = (i + 1) % polygon.size;

		point2D a = polygon.points[i];
		point2D b = polygon.points[j];
		//point2D a = localToWorld_point2D(polygon.points[i], *ref);
		//point2D b = localToWorld_point2D(polygon.points[j], *ref);

		SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
	}
}

void polygon_render(SDL_Renderer* renderer, polygon* polygons, referential* ref)
{
    for (unsigned int i = 0; i < polygons->nbConvexPolygons; i++)
    {
        convexPolygon_render(renderer, polygons->convexPolygons[i], ref);
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

