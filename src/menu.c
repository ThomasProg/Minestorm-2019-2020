#include <assert.h>

#include "menu.h"
#include "macros.h"
#include "widgets/button.h"
#include "widgets/textbox.h"

t_menu* menu_create(t_assets* assets)
{
    t_menu* menu = malloc(sizeof(t_menu));

	SDL_Rect rect;
	rect.x = WINDOW_SIZE_X / 2;
	rect.y = 60;
	rect.w = 100;
	rect.h = 100;

	menu->button = button_init(render_get(assets->render, 0), rect, (SDL_Rect){0, 0, 500, 500});

	rect.y = 10;
	TTF_Font* font = font_get(assets->font, 0);
	menu->textbox = textbox_create("MINESTORM MENU", rect, font);

	rect.y += 400;
	menu->helpTextBox = textbox_create("Press f to play solo \n Press k to play with 2 players \n Esc to leave", rect, font);

    return menu;
}

void menu_destroy(t_menu* menu)
{
	textbox_destroy(menu->textbox);
	free(menu->textbox);

	textbox_destroy(menu->helpTextBox);
	free(menu->helpTextBox);
	
	button_destroy(menu->button);
	free(menu->button);

    free(menu);
}

void menu_render(t_menu* menu, t_assets* assets)
{
	button_render(assets->render->renderer, menu->button);

	textbox_render(assets->render->renderer, menu->textbox);
	textbox_render(assets->render->renderer, menu->helpTextBox);
}

void menu_inputs(t_level* level, t_menu* menu)
{
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		menu->mouseLocation.x = event.button.x;
		menu->mouseLocation.y = event.button.y;

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (button_collision(menu->button, menu->mouseLocation.x, menu->mouseLocation.y))
				{
					level_modifySafe(level, E_PLAY);
				}
			}
		}

		if (event.type == SDL_KEYDOWN) //press
		{
			int key = event.key.keysym.sym;

			if (key == SDLK_f)
			{
				level_modifySafe(level, E_PLAY);
			}
			if (key == SDLK_k)
			{
				level_modifySafe(level, E_PLAY2);	
			}

			if (key == SDLK_ESCAPE)
				level_modifySafe(level, E_QUIT);

		}

		if (event.type == SDL_QUIT)
			level_modifySafe(level, E_QUIT);
	}
}


void menu_loop(t_assets* assets, float deltaTime, t_level* level)
{
	t_menu* menu = ((t_menu*) (level->data));

	menu_render(menu, assets);

	menu_inputs(level, menu);
}
