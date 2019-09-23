#include "game.h"

#include "SDL_utilities/SDL_utilities.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"
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
		
	//SDL_Surface* surface = IMG_Load("media/Stickman_Sprite_Sheet.png");
		SDL_Texture* texture = render_get(game->assets->render, 0);//SDL_CreateTextureFromSurface(game->assets->render->renderer, surface);
		//SDL_FreeSurface(surface);
	SDL_Rect rect = {0, 0, 50, 100};
	SDL_Rect src = {0, 100, 50, 100};
	SDL_RenderCopy(renderer, texture, &src, &rect);

		level_tick(&game->level, game->assets, deltaTime);

		if (game->level.levelID == E_QUIT)
		{
			game->run = false;
		}

		SDL_RenderPresent(renderer);

		SDL_Delay(1);
	}
}

bool game_init(game* game)
{
	srand(time(NULL));   // Initialization, should only be called once.

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) 
		return 0;

	t_assets* assets = assets_create(50, 4, 1); //50 images, 4 sounds, 1 font
	
	render_add(assets->render, "media/Stickman_Sprite_Sheet.png");
	//audio_add(assets->audio, "media/GourmetRace.wav");
	//font_add(assets->font, "media/arial.ttf", 10);

	game->assets = assets;
	//game->level.levelID = E_PLAY;
	level_init(&game->level, E_MENU);//E_PLAY);
	game->run = true;
	return 1;
}

void game_destroy(game* game)
{
	assets_destroy(game->assets);
	level_destroy(&game->level);
	//free(game->assets);

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
	// srand(time(NULL));   // Initialization, should only be called once.

	// if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) 
	// 	return 0;

	// t_assets* assets = assets_create(50, 4, 1); //50 images, 4 sounds, 1 font
	
	// render_add(assets->render, "media/Stickman_Sprite_Sheet.png");
	// //audio_add(assets->audio, "media/GourmetRace.wav");
	// //font_add(assets->font, "media/arial.ttf", 10);
	// //
	// //t_entity* entity = entity_create();
	// t_player* player = player_create();
	// player->inputValues = getInputValues1();

	// t_player* player2 = player_create();
	// player2->inputValues = getInputValues2();

	// t_floatingMine* floatingMine = floatingMine_create();
	// t_magneticMine* magneticMine = magneticMine_create();
	// t_fireballMine* fireballMine = fireballMine_create();
	// //t_bullet** bullets = malloc(sizeof(t_bullet));

	// magneticMine->target = player;
	
	// game game;
	// game.level.levelID = E_MENU;
	// game.run = true;
	// int lastTime = 0;
	// while (game.run)
	// {
	// 	//clear_screen();
	// 	SDL_SetRenderDrawColor(assets->render->renderer, 0x0F, 0x0F, 0x0F, 0xFF);
	// 	SDL_RenderClear(assets->render->renderer);
		
	// 	int time = SDL_GetTicks();
	// 	float deltaTick = (time - lastTime) / 1000.f;

	// 	player_tick(player, deltaTick);
	// 	player_tick(player2, deltaTick);
	// 	floatingMine_tick(floatingMine, deltaTick);
	// 	magneticMine_tick(magneticMine, deltaTick);
	// 	fireballMine_tick(fireballMine, deltaTick);

	// 	lastTime = time;

	// 	SDL_Event event;
	// 	while (SDL_PollEvent(&event))
	// 	{
	// 		if (event.type == SDL_KEYDOWN) //click
	// 		{
	// 			int key = event.key.keysym.sym;
	// 			if (key == SDLK_ESCAPE)
	// 				game.run = false;

	// 			player_input_start(player, key, true);
	// 			player_input_start(player2, key, true);
	// 		}
	// 		if (event.type == SDL_KEYUP) //release
	// 		{
	// 			int key = event.key.keysym.sym;
	// 			player_input_start(player, key, false);
	// 			player_input_start(player2, key, false);
	// 		}
	// 		if (event.type == SDL_QUIT)
	// 			game.run = false;
	// 	}

	// 	//player_event(player, bullets);
	// 	//player_event(player2, bullets);

	// 	//t_bullet* bullet = bullet_create();
	// 	//entity_render(entity, assets->render);
	// 	floatingMine_render(floatingMine, assets->render);
	// 	magneticMine_render(magneticMine, assets->render);
	// 	fireballMine_render(fireballMine, assets->render);
	// 	player_render(player, assets->render);
	// 	player_render(player2, assets->render);
	// 	//bullet_render(assets->render->renderer, bullets[0], 10);

	// 	{
	// 		convexPolygon* playerColl = &((polygon*) player->entity.collision)->convexPolygons[0];
	// 		convexPolygon* mineColl = &((polygon*) floatingMine->entity.collision)->convexPolygons[0];

	// 		convexPolygon p1, p2;
	// 		p1.size = playerColl->size;
	// 		p1.points = malloc(playerColl->size * sizeof(point2D));
	// 		p2.size = mineColl->size;
	// 		p2.points = malloc(mineColl->size * sizeof(point2D));

	// 		for (unsigned int k = 0; k < playerColl->size; k++)
	// 		{
	// 			p1.points[k] = localToWorld_point2D(playerColl->points[k], player->entity.ref);
	// 		}

	// 		for (unsigned int k = 0; k < mineColl->size; k++)
	// 		{
	// 			p2.points[k] = localToWorld_point2D(mineColl->points[k], floatingMine->entity.ref);
	// 		}

	// 		if (convexPolygon_convexPolygon_collision(p1, p2))
	// 		{
	// 			//fprintf(stderr, "COLLISION\n");
	// 		}
	// 	}

	// 	// //bullet collision
	// 	// {
	// 	// 	if (circle_convexPolygon_collision(bullets[0]->collision, ((convexPolygonsArray*) fireballMine->entity.collision)->polygons[0]))
	// 	// 	{
	// 	// 		//fprintf(stderr, "game.c:125 / circle_convexPolygon_collision\n");
	// 	// 	}
	// 	// }

	// 	SDL_RenderPresent(assets->render->renderer);

	// 	SDL_Delay(1);
	// }

	// assets_destroy(assets);

	// SDL_Quit();
}


