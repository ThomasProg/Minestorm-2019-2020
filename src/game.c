#include "game.h"

#include "SDL_utilities/SDL_utilities.h"
#include "entities.h"

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) 
		return 0;

	t_assets* assets = assets_create(50, 4, 1); //50 images, 4 sounds, 1 font
	
	render_add(assets->render, "media/Stickman_Sprite_Sheet.png");
	//audio_add(assets->audio, "media/GourmetRace.wav");
	//font_add(assets->font, "media/arial.ttf", 10);
	//
	t_entity* entity = entity_create();

	if (render_get(assets->render, 0) != NULL)
		printf("DATA\n");
	
	//TODO : gamestate
	int run = 1;
	float lastTime = 0;
	while (run)
	{
		//clear_screen();
		SDL_SetRenderDrawColor(assets->render->renderer, 0x3F, 0x3F, 0x3F, 0xFF);
		SDL_RenderClear(assets->render->renderer);
		
		int time = SDL_GetTicks();
		while (time - lastTime > 15)
		{
			entity_tick(entity);
			lastTime += 15;
			//lastTime = time;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN) //click
			{
				int key = event.key.keysym.sym;
				entity_input(entity, key);
			}
			if (event.type == SDL_KEYUP) //release
			{
				int key = event.key.keysym.sym;
				key = key;
			}
			if (event.type == SDL_QUIT)
				run = 0;
		}

		//render_things();
		entity_render(entity, assets->render);
		SDL_RenderPresent(assets->render->renderer);

		SDL_Delay(1);
	}

	assets_destroy(assets);

	SDL_Quit();
}


