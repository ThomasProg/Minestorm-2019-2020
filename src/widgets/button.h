#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct
{
	SDL_Rect destination;
	SDL_Rect src;
	SDL_Texture* texture;
} t_image;

typedef t_image t_button;

t_button* button_init(SDL_Texture* texture, SDL_Rect dest, SDL_Rect src);

void button_destroy(t_button* button);

void button_render(SDL_Renderer* renderer, t_button* button);

bool button_collision(t_button* button, int x, int y);


#endif
