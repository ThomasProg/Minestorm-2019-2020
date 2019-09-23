#include <assert.h>
#include <string.h>

#include "world.h"
#include "entities/entities.h"
#include "entities/player.h"
#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"
#include "entities/spawner.h"
#include "worldField.h"
#include "entities/bullets.h"
#include "vector2D/collisions2D.h"
#include "string/string_utilities.h"
#include "macros.h"

t_world* world_create(t_assets* assets)
{
    t_world* world = malloc(sizeof(t_world));

	TTF_Font* font = font_get(assets->font, 0);
	SDL_Rect rect = {50, 50, 500, 500};
	world->widgets.scoreTextBox = textbox_create("SCORE : ", rect, font);

	//init Players
	dynamicArray_Init(&world->players, sizeof(t_player), 2);
	for (unsigned int i = 0; i < 2; i++)
	{
		t_player* player = dynamicArray_AddItem(&world->players);

		player_init(player);
		player->inputValues = getInputValues(i % 2);
	}

	dynamicArray_Init(&world->mines, sizeof(t_mine), 4);
	dynamicArray_Init(&world->bullets, sizeof(t_bullet), 16);
	dynamicArray_Init(&world->spawners, sizeof(t_spawner), 4);

	world->isPaused = false;

	newWorldLevel(world, 1);

    return world;
}

void world_destroy(t_world* world)
{
	textbox_destroy(world->widgets.scoreTextBox);
	free(world->widgets.scoreTextBox);

	dynamicArray_Destroy(&world->mines);
	//dynamicArray_Destroy(&world->floatingMines);
	//dynamicArray_Destroy(&world->magneticMines);
	//dynamicArray_Destroy(&world->fireballMines);

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
	float randNumCumulated = 5.0f + randNum / 10.f;

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

void world_render(t_world* world, t_assets* assets)
{
	t_player* player = dynamicArray_GetItem(&world->players, 0);
	char* str = string_append("SCORE : ", int_to_alpha(player->score));
	textbox_setText(world->widgets.scoreTextBox, str);
	free(str);
	textbox_render(assets->render->renderer, world->widgets.scoreTextBox);

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, i);
		mine_render(mine, assets->render);
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		player_render(player, assets->render);
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

		if (player->bInputs.shoot)
		{
			vector2D bulletLocation = player->entity.ref.origin;
			bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, 100.0 * 0.25 + 10));

			t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

			bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI);
			player->bInputs.shoot = false;
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
				break;
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
				fprintf(stderr, "OOH WILFRE!\n");
			}
			else
			{
				fprintf(stderr, "no collision !\n");
			}
		}
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
}
