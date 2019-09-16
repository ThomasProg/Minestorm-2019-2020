#include "render.h"
#include "macros.h"
#include <SDL2/SDL_image.h>

t_render* render_create(const unsigned int nbTextures)
{
	t_render* render = malloc(sizeof(*render));
	render->textures = queue_create(nbTextures, sizeof(SDL_Texture*));
	render->window = SDL_CreateWindow("MINESTORM", SDL_WINDOWPOS_CENTERED, SDL_WINDOW_SHOWN, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
	render->renderer = SDL_CreateRenderer(render->window, -1, SDL_RENDERER_ACCELERATED);

	return render;
}

void render_add(t_render* render, const char* filename)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface != NULL)
	{
		queue_add(render->textures);
		SDL_Texture** elems = render->textures->data;
		elems[render->textures->nextIndex] = SDL_CreateTextureFromSurface(			render->renderer, surface);
		render->textures->nextIndex++;

	}
}

void render_destroy(t_render* render)
{
	SDL_Texture** textures;

	for (unsigned int i = 0; i < render->textures->nextIndex; i++)
	{
		textures = render->textures->data;
		if (textures[i] != NULL)
			SDL_DestroyTexture(textures[i]);
	}
	
	queue_destroy(render->textures);
	free(render);
}

SDL_Texture* render_get(t_render* render, unsigned int i)
{
	return ((SDL_Texture**)(render->textures->data))[i];
}
