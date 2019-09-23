#include "button.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

t_button* button_init(SDL_Texture* texture, SDL_Rect dest)
{
	t_button* button = malloc(sizeof(t_button));

	button->texture = texture;
	button->destination = dest;

	return button;
}

void button_destroy(t_button* button)
{
	//SDL_DestroyTexture(button->texture);
}

void button_render(SDL_Renderer* renderer, t_button* button)
{
	//fprintf(stderr, "RENDER");
	//SDL_Rect rect = {0, 0, 50, 100};
	//SDL_Rect src = {0, 100, 50, 100};

		//SDL_Surface* surface = IMG_Load("media/Stickman_Sprite_Sheet.png");
		//SDL_Texture* texture = render_get(game->assets->render, 0);//SDL_CreateTextureFromSurface(game->assets->render->renderer, surface);
		//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		//SDL_FreeSurface(surface);

	//button->texture = texture;
	//SDL_RenderCopy(renderer, button->texture, &src, &rect);
	SDL_RenderCopy(renderer, button->texture, &button->destination, &button->destination);

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
