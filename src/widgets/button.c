#include "button.h"
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

t_button* button_init(SDL_Texture* texture, SDL_Rect rect)
{
	t_button* button = malloc(sizeof(t_button));

	button->texture = texture;
	button->destination = rect;

	return button;
}

void button_destroy(t_button* button)
{
	SDL_DestroyTexture(button->texture);
}

void button_render(SDL_Renderer* renderer, t_button* button)
{
	SDL_Rect rect = {0, 0, 50, 100};
	SDL_Rect src = {0, 100, 50, 100};
	SDL_RenderCopy(renderer, button->texture, &src, &rect);
	//SDL_RenderCopy(renderer, button->texture, &button->destination, &button->destination);

}

bool button_collision(t_button* button, int x, int y)
{

	return x > button->destination.x
	    && y > button->destination.y;
}
