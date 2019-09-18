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

int main()
{
	srand(time(NULL));   // Initialization, should only be called once.

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) 
		return 0;

	t_assets* assets = assets_create(50, 4, 1); //50 images, 4 sounds, 1 font
	
	render_add(assets->render, "media/Stickman_Sprite_Sheet.png");
	//audio_add(assets->audio, "media/GourmetRace.wav");
	//font_add(assets->font, "media/arial.ttf", 10);
	//
	//t_entity* entity = entity_create();
	t_player* player = player_create();

	t_floatingMine* floatingMine = floatingMine_create();
	t_magneticMine* magneticMine = magneticMine_create();
	t_fireballMine* fireballMine = fireballMine_create();
	t_bullet** bullets = malloc(sizeof(t_bullet));

	magneticMine->target = player;
	
	//TODO : gamestate
	int run = 1;
	int lastTime = 0;
	while (run)
	{
		//clear_screen();
		SDL_SetRenderDrawColor(assets->render->renderer, 0x0F, 0x0F, 0x0F, 0xFF);
		SDL_RenderClear(assets->render->renderer);
		
		int time = SDL_GetTicks();
		// while (time - lastTime > 15)
		// {
			//entity_tick(entity);
			float deltaTick = (time - lastTime) / 1000.f;
			player_tick(player, deltaTick);
			floatingMine_tick(floatingMine, deltaTick);
			magneticMine_tick(magneticMine, deltaTick);
			fireballMine_tick(fireballMine, deltaTick);
			bullet_tick(bullets[0], deltaTick);
			//lastTime = time;
		//}
		lastTime = time;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN) //click
			{
				int key = event.key.keysym.sym;
				player_input_start(player, key, true);
			}
			if (event.type == SDL_KEYUP) //release
			{
				int key = event.key.keysym.sym;
				player_input_start(player, key, false);
			}
			if (event.type == SDL_QUIT)
				run = 0;
		}

		player_event(player, bullets);

		//t_bullet* bullet = bullet_create();
		//entity_render(entity, assets->render);
		floatingMine_render(floatingMine, assets->render);
		magneticMine_render(magneticMine, assets->render);
		fireballMine_render(fireballMine, assets->render);
		player_render(player, assets->render);
		bullet_render(assets->render->renderer, bullets[0], 10);

		{
			convexPolygon* playerColl = &((convexPolygonsArray*) player->entity.collision)->polygons[0];
			convexPolygon* mineColl = &((convexPolygonsArray*) floatingMine->entity.collision)->polygons[0];

			convexPolygon p1, p2;
			p1.size = playerColl->size;
			p1.points = malloc(playerColl->size * sizeof(point2D));
			p2.size = mineColl->size;
			p2.points = malloc(mineColl->size * sizeof(point2D));

			for (unsigned int k = 0; k < playerColl->size; k++)
			{
				p1.points[k] = localToWorld_point2D(playerColl->points[k], player->entity.ref);
			}

			for (unsigned int k = 0; k < mineColl->size; k++)
			{
				p2.points[k] = localToWorld_point2D(mineColl->points[k], floatingMine->entity.ref);
			}

			if (convexPolygon_convexPolygon_collision(p1, p2))
			{
				//fprintf(stderr, "COLLISION\n");
			}
		}

		//bullet collision
		{
			if (circle_convexPolygon_collision(bullets[0]->collision, ((convexPolygonsArray*) fireballMine->entity.collision)->polygons[0]))
			{
				fprintf(stderr, "REMOVE\n");
			}
		}

		SDL_RenderPresent(assets->render->renderer);

		SDL_Delay(1);
	}

	assets_destroy(assets);

	SDL_Quit();
}


