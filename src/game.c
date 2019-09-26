#include "game.h"

#include "SDL_utilities/SDL_utilities.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/bullets.h"
#include "vector2D/collisions2D.h"

#include <time.h>
#include <SDL2/SDL_image.h>

#include "world.h"
#include "menu.h"

void game_loop(game* game)
{
	SDL_Renderer* renderer = game->assets->render->renderer;
	int lastTime = 0.0, time;
	float deltaTime;

	while (game->run)
	{
		time = SDL_GetTicks();
		deltaTime = (time - lastTime) / 1000.f;
		lastTime = time;

		//clear_screen();
		SDL_SetRenderDrawColor(renderer, 0x00, 0x10, 0x10, 0xFF);
		SDL_RenderClear(renderer);
		
		level_tick(&game->level, game->assets, deltaTime);

		if (game->level.levelID == E_QUIT)
		{
			game->run = false;
		}

		SDL_RenderPresent(renderer);
	}
}

bool game_init(game* game)
{
	srand(time(NULL));   // Initialization, should only be called once.

	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
		return false;

	if (TTF_Init() < 0)
		return false;
	
	t_assets* assets = assets_create(1, 0, 1); //1 images, 0 sounds, 1 font
	
	render_add(assets->render, "media/start.jpg");
	font_add  (assets->font, "media/font.ttf", 20);

	game->assets = assets;
	level_init(&game->level, E_MENU, assets);
	game->run = true;
	return true;
}

void game_destroy(game* game)
{
	assets_destroy(game->assets);
	level_destroy(&game->level);

	TTF_Quit();
	SDL_Quit();
}

void startGame()
{
	game game;
	if (!game_init(&game))
		return; //return if fail to load SDL
	
	game_loop(&game);

	game_destroy(&game);
}

int main()
{
	startGame();
	return 0;
}


