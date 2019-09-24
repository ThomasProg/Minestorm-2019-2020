#ifndef _RENDER_SHAPES_H_
#define _RENDER_SHAPES_H_

#include <SDL2/SDL.h>

#include "vector2D/vector2D.h"
#include "vector2D/objects.h"

void axisAlignedRectangle_render(SDL_Renderer* renderer, axisAlignedRectangle rectangle);

void convexPolygon_render(SDL_Renderer* renderer, convexPolygon polygon, bool renderAABB);
void polygon_render(SDL_Renderer* renderer, polygon* polygons, bool renderAABB);

#endif