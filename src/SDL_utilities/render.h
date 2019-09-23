#ifndef _RENDER_H_
#define _RENDER_H_

#include <SDL2/SDL.h>

#include "queue.h"

typedef struct s_render
{
	t_queue*      textures;
	//t_dynamicArray textures;
	SDL_Renderer* renderer;
	SDL_Window*   window;
} t_render;

t_render* render_create(const unsigned int nbTextures);
void      render_add(t_render* render, const char* filename);
SDL_Texture* render_get(t_render* render, unsigned int i);
void      render_destroy(t_render* render);


#endif
