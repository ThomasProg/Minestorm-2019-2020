#include "button.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

t_button* button_init(SDL_Texture* texture, SDL_Rect dest, SDL_Rect src)
{
	t_button* button = malloc(sizeof(t_button));

	button->texture = texture;
	button->destination = dest;
	button->src = src;

	return button;
}

void button_destroy(t_button* button)
{
	//SDL_DestroyTexture(button->texture);
}

void button_render(SDL_Renderer* renderer, t_button* button)
{
	SDL_RenderCopy(renderer, button->texture, &button->src, &button->destination);
}

bool button_collision(t_button* button, int x, int y)
{
	return //x collision
		   x > button->destination.x  
		&& x < button->destination.x + button->destination.w
		   //y collision
	    && y > button->destination.y 
		&& y < button->destination.y + button->destination.h;
}
