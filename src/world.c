#include <assert.h>
#include <string.h>

#include "world.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/spawner.h"
#include "worldField.h"
#include "entities/bullets.h"
#include "vector2D/collisions2D.h"
#include "string/string_utilities.h"
#include "macros.h"

t_world* world_create(t_assets* assets)
{
    t_world* world = malloc(sizeof(t_world));

	TTF_Font* font 		  = font_get(assets->font, 0);
	SDL_Rect rectScore    = {150, 0, 500, 500};
	SDL_Rect rectLife     = {150, 40, 500, 500};
	SDL_Rect rectGameOver = {WINDOW_SIZE_X/2, 150, 500, 500};

	world->widgets.scoreTextBox1 = textbox_create("SCORE : ", rectScore, font);
	world->widgets.lifeTextBox1  = textbox_create("LIFE : 3", rectLife, font);

	rectScore.x = WINDOW_SIZE_X - 150;  
	rectLife.x  = WINDOW_SIZE_X - 150;

	world->widgets.scoreTextBox2   = textbox_create("SCORE : ", rectScore, font);
	world->widgets.lifeTextBox2    = textbox_create("LIFE : 3", rectLife, font);
	world->widgets.gameOverTextBox = textbox_create("GAME OVER\n Click on escape to return to the menu", rectGameOver, font);

	//init Players
	dynamicArray_Init(&world->players, sizeof(t_player), NB_PLAYERS);
	for (unsigned int i = 0; i < NB_PLAYERS; i++)
	{
		t_player* player = dynamicArray_AddItem(&world->players);

		player_init(player);
		player->inputValues = getInputValues(i % NB_PLAYERS);
	}

	dynamicArray_Init(&world->mines, sizeof(t_mine), 4);
	dynamicArray_Init(&world->bullets, sizeof(t_bullet), 16);
	dynamicArray_Init(&world->spawners, sizeof(t_spawner), 4);

	world->isPaused = false;
	world->debugMode = false;

	newWorldLevel(world, 1);

    return world;
}

void world_destroy(t_world* world)
{
	textbox_destroy(world->widgets.gameOverTextBox);
	free(world->widgets.gameOverTextBox);

	textbox_destroy(world->widgets.lifeTextBox1);
	free(world->widgets.lifeTextBox1);
	textbox_destroy(world->widgets.scoreTextBox1);
	free(world->widgets.scoreTextBox1);

	textbox_destroy(world->widgets.lifeTextBox2);
	free(world->widgets.lifeTextBox2);
	textbox_destroy(world->widgets.scoreTextBox2);
	free(world->widgets.scoreTextBox2);

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		mine_destroy(dynamicArray_GetItem(&world->mines, i));
	}

	dynamicArray_Destroy(&world->mines);

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		player_destroy(dynamicArray_GetItem(&world->players, i));
	}

	dynamicArray_Destroy(&world->bullets);
	dynamicArray_Destroy(&world->spawners);
	dynamicArray_Destroy(&world->players);

    free(world);
}

void newWorldLevel(t_world* world, unsigned int level)
{
	//set spawners
	unsigned int nbSpawners = pow((level % 3), 1.1) + 2 + (level / 3);
	nbSpawners = 5;

	t_spawner* spawner;

	unsigned int randNum = rand() % 10;
	float randNumCumulated = 1.0f + randNum / 10.f;

	for (unsigned int i = 0; i < nbSpawners; i++)
	{
		spawner = dynamicArray_AddItem(&world->spawners);
		spawner->location = getRandomLocation();
		spawner->size = BIG;
		spawner->delay = randNumCumulated;
		spawner->mineType = i % 4;
		randNumCumulated += randNum * 2;
		//spawner->size = 0.8f;
	}
}

void widgets_render(t_world* world, t_assets* assets, unsigned int playerID)
{
	{
		t_player* player = dynamicArray_GetItem(&world->players, playerID);
		char* score = int_to_alpha(player->score);
		char* str = string_append("SCORE : ", score);
		t_textbox* textbox;
		if (playerID == 0)
			textbox = world->widgets.scoreTextBox1;
		else
			textbox = world->widgets.scoreTextBox2;
		
		free(score);
		textbox_setText(textbox, str);
		free(str);
		textbox_render(assets->render->renderer, textbox);
	}
	{
		t_player* player = dynamicArray_GetItem(&world->players, playerID);
		char* life = int_to_alpha(player->life);
		char* str = string_append("LIFE : ", life);
		t_textbox* textbox;
		if (playerID == 0)
			textbox = world->widgets.lifeTextBox1;
		else
			textbox = world->widgets.lifeTextBox2;

		free(life);
		textbox_setText(textbox, str);
		free(str);
		textbox_render(assets->render->renderer, textbox);
	}
}

void world_render(t_world* world, t_assets* assets)
{
	if (dynamicArray_IsValidIndex(&world->players, 0))
		widgets_render(world, assets, 0);
	if (dynamicArray_IsValidIndex(&world->players, 1))
	{
		widgets_render(world, assets, 1);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, i);
		mine_render(mine, assets->render, world->debugMode);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		player_render(player, assets->render, world->debugMode);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->bullets, &i); i++)
	{
		t_bullet* bullet = dynamicArray_GetItem(&world->bullets, i);
		bullet_render(assets->render->renderer, bullet, BULLETS_PRECISION);
	}
}

void world_tick(t_world* world, float deltaTime)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		player_tick(player, deltaTime);

		if (player->bInputs.shoot && SDL_GetTicks() - player->lastShoot > SHOOT_INTERVAL)
		{
			vector2D bulletLocation = player->entity.ref.origin;
			bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0 * 0.25 + 10));

			t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

			bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI);
			player->bInputs.shoot = false;
			player->lastShoot = SDL_GetTicks();
		}
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, i);
		mine_tick(mine, deltaTime);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->bullets, &i); i++)
	{
		t_bullet* bullet = dynamicArray_GetItem(&world->bullets, i);
		bullet_tick(bullet, deltaTime);

		bullet = dynamicArray_GetItem(&world->bullets, i);

		bullet_tick(bullet, deltaTime);
		if (bullet->timeAlive > BULLETS_TIME_ALIVE)
		{
			bullet_destroy(bullet);
			dynamicArray_RemoveItem(&world->bullets, i);
		}
	}
}

void world_inputs(t_level* level, t_world* world)
{
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN) //press
		{
			int key = event.key.keysym.sym;
			if (key == SDLK_SPACE)
			{
				world->isPaused = !world->isPaused;
			}
			if (key == SDLK_c)
			{
				world->debugMode = !world->debugMode;
			}

			if (key == SDLK_ESCAPE)
				level_modifySafe(level, E_MENU);

			for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
			{
				player_input_start(dynamicArray_GetItem(&world->players, i), key, true);
			}
		}
		if (event.type == SDL_KEYUP) //release
		{
			int key = event.key.keysym.sym;


			for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
			{
				player_input_start(dynamicArray_GetItem(&world->players, i), key, false);
			}
		}
		if (event.type == SDL_QUIT)
			level_modifySafe(level, E_QUIT);
			//level->quit = true;
			//level_modify(level, E_QUIT);
			//game->run = false;
	}
}

t_bullet* bullets_polygon_collision(t_dynamicArray* bullets, polygon* polygon, unsigned int* bulletID)
{
	for (unsigned int k = 0; dynamicArray_GetValidItemIndex(bullets, &k); k++)
	{
		t_bullet* bullet = dynamicArray_GetItem(bullets, k);
		if (circle_polgyon_collision(&bullet->collision, polygon))
		{
			if (bulletID != NULL)
				*bulletID = k;

			return bullet;
		}
	}

	return NULL;
}

void world_collisions(t_world* world)
{
	for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->mines, &j); j++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, j);

		unsigned int bulletID = 0;
		t_bullet* bullet = bullets_polygon_collision(&world->bullets, &mine->entity.worldCollider, &bulletID);
		
		if (bullet != NULL)
		{
			bullet_destroy(bullet);
			dynamicArray_RemoveItem(&world->bullets, bulletID);

			if (mine->throwFireballs)
			{
				vector2D bulletLocation = mine->entity.ref.origin;
				bulletLocation = addVectors(bulletLocation, scaleVector(mine->entity.ref.unitI, 100.0 * 0.25 + 10));

				t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

				vector2D shootDirection = substractVectors(mine->target->entity.ref.origin, mine->entity.ref.origin);
				
				if (! floatIsNearlyEqual(vectorLength(shootDirection), 0, 0.001f))
					bullet_init(bullet, bulletLocation, mine->entity.velocity, unitVector(shootDirection));

			}

			if (mine->sizeType > SMALL)
			{
				t_spawner* spawner;
				for (unsigned int i = 0; i < 2; i++)
				{
					spawner = dynamicArray_AddItem(&world->spawners);
					spawner->location = getRandomLocation();
					spawner->size = mine->sizeType - 1;
					spawner->mineType = mine->throwFireballs + mine->followPlayer * 2;
				}
			}

			t_player* player = dynamicArray_GetItem(&world->players, 0);
			player->score += mine->givenScore;

			mine_destroy(mine);

			dynamicArray_RemoveItem(&world->mines, j);
		}
	}

	//for each player
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		
		for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->mines, &j); j++)
		{
			t_mine* mine = dynamicArray_GetItem(&world->mines, j);
			t_entity* entity = &mine->entity;
			
			if (polygon_polgyon_collision(&player->entity.worldCollider, &entity->worldCollider))
			{
				//player damage
				if (player->life == 1)
				{
					player_destroy(player);
					dynamicArray_RemoveItem(&world->players, i);
				}
				else
				{
					entity_teleport(&player->entity);
				}
				if (player->life != 0)
					player->life--;	
			}
		}
	}
}

void world_GameOver(t_world* world, t_level* level, SDL_Renderer* renderer)
{
	unsigned int playerID;
	bool isOver = dynamicArray_GetValidItemIndex(&world->players, &playerID);

	if (!isOver)
	{
		textbox_render(renderer, world->widgets.gameOverTextBox);
	}
}

void world_loop(t_assets* assets, float deltaTime, t_level* level)
{
    t_world* world = ((t_world*) (level->data));

	//if (world->spawners.usedItems > 0)
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->spawners, &i); i++)
	{
		t_spawner* spawner = dynamicArray_GetItem(&world->spawners, i);

		if (spawner->delay < 0)
		{
			t_mine* mine = dynamicArray_AddItem(&world->mines);
			mine_init(mine, spawner->mineType, spawner->location, spawner->size);
			mine->target = dynamicArray_GetItem(&world->players, 0);

			dynamicArray_RemoveItem(&world->spawners, i);
			break;
		}
		else
		{
			spawner->delay -= deltaTime;
			int x1, y1, x2, y2;
			x1 = spawner->location.x - 10;
			y1 = spawner->location.y - 10;
			x2 = spawner->location.x + 10;
			y2 = spawner->location.y + 10;
			SDL_SetRenderDrawColor(assets->render->renderer, 50, 50, 50, 0);
			SDL_RenderDrawLine(assets->render->renderer, x1, y1, x2, y2);
			SDL_RenderDrawLine(assets->render->renderer, x1, y2, x2, y1);
		}
		
	 }

	world_render(world, assets);

	world_inputs(level, world);

	if (world->isPaused)
		return;

	world_tick(world, deltaTime);

	world_collisions(world);

	world_GameOver(world, level, assets->render->renderer);
}
