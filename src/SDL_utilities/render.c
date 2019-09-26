#include "render.h"
#include "macros.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

t_render* render_create(const unsigned int nbTextures)
{
	t_render* render = malloc(sizeof(*render));
	dynamicArray_Init(&render->textures, sizeof(SDL_Texture*), nbTextures);
	render->window = SDL_CreateWindow("MINESTORM", SDL_WINDOWPOS_CENTERED, SDL_WINDOW_SHOWN, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
	render->renderer = SDL_CreateRenderer(render->window, -1, SDL_RENDERER_ACCELERATED);

	return render;
}

void render_add(t_render* render, const char* filename)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface != NULL)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(render->renderer, surface);
		SDL_Texture** tex = dynamicArray_AddItem(&render->textures);
		*tex = texture;

		SDL_FreeSurface(surface);

	}
}

void render_destroy(t_render* render)
{
	for (unsigned int i = 0; i < dynamicArray_GetValidItemIndex(&render->textures, &i); i++)
	{
		SDL_Texture* texture = render_get(render, i);
		if (texture != NULL)
			SDL_DestroyTexture(texture);
	}
	
	dynamicArray_Destroy(&render->textures);
	SDL_DestroyRenderer(render->renderer);
	free(render);
}

SDL_Texture* render_get(t_render* render, unsigned int i)
{
	return *((SDL_Texture**) dynamicArray_GetItem(&render->textures, i));
}
