#include <assert.h>

#include "menu.h"
#include "macros.h"
#include "widgets/button.h"

t_menu* menu_create()
{
    t_menu* menu = malloc(sizeof(t_menu));


    return menu;
}

void menu_destroy(t_menu* menu)
{

    free(menu);
}

void menu_render(t_menu* menu, t_assets* assets)
{
	SDL_Rect rect;
	rect.x = 10;
	rect.y = 10;
	rect.w = 100;
	rect.h = 100;
	t_button* button = button_init(render_get(assets->render, 0), rect);
	button_render(assets->render->renderer, button);
	button_destroy(button);
}

void menu_tick(t_menu* menu, float deltaTime)
{

}

void menu_inputs(game* game, t_menu* menu)
{
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN) //press
		{
			int key = event.key.keysym.sym;
			if (key == SDLK_SPACE)
			{
				//world->isPaused = !world->isPaused;
				//break;
				level_modify(&game->level, E_PLAY);
			}

			if (key == SDLK_ESCAPE)
				game->run = false;

		}
		if (event.type == SDL_KEYUP) //release
		{
			//int key = event.key.keysym.sym;


		}
		if (event.type == SDL_QUIT)
			game->run = false;
	}
}


void menu_loop(t_assets* assets, float deltaTime, game* game)
{
	t_menu* menu = ((t_menu*) (game->level.data));

	menu_render(menu, assets);

	menu_inputs(game, menu);

	//if (world->isPaused)
	//	return;

	//world_tick(world, deltaTime);

}
