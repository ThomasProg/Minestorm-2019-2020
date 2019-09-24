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

t_minesToSpawn minesToSpawn_create()
{
	t_minesToSpawn returned;
	returned.floatingMine = 0;
	returned.fireballMine = 0;
	returned.magneticMine = 0;
	returned.magneticFireballMine = 0;
	return returned;
}

t_minesToSpawn minesToSpawn_getMax(unsigned int lvl)
{
	t_minesToSpawn returned = minesToSpawn_create();
	returned.magneticFireballMine = (lvl / 8);
	returned.fireballMine = (lvl / 4) - 4 * (lvl / 8);
	returned.magneticMine = (lvl / 2) - 2 * (lvl / 4);
	returned.floatingMine =  (lvl / 1) - 2 * (lvl / 2);

	return returned;
}

t_world* world_create(t_assets* assets, unsigned int nbPlayers)
{
    t_world* world = malloc(sizeof(t_world));

	//world->maxMines     = minesToSpawn_getMax(1);
	world->currentPlayLevel  = 3;
	world->minesBySize = malloc(NB_MINE_SIZES * sizeof(t_minesToSpawn));


    world->spawnDelay   = 3.f;

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
	world->widgets.pauseTextBox    = textbox_create("PAUSE\n Press SPACE to resume", rectGameOver, font);

	//init Players
	dynamicArray_Init(&world->players, sizeof(t_player), nbPlayers);
	for (unsigned int i = 0; i < nbPlayers; i++)
	{
		t_player* player = dynamicArray_AddItem(&world->players);

		player_init(player);
		player->inputValues = getInputValues(i % nbPlayers);
	}

	dynamicArray_Init(&world->mines, sizeof(t_mine), 4);
	dynamicArray_Init(&world->bullets, sizeof(t_bullet), 16);
	dynamicArray_Init(&world->spawners, sizeof(t_spawner), 10);

	world->isPaused = false;
	world->debugMode = false;

	//newWorldLevel(world, 1);

    return world;
}

void world_destroy(t_world* world)
{
	textbox_destroy(world->widgets.gameOverTextBox);
	free(world->widgets.gameOverTextBox);
	textbox_destroy(world->widgets.pauseTextBox);
	free(world->widgets.pauseTextBox);

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

	free(world->minesBySize);

    free(world);
}

void newWorldLevel(t_world* world, unsigned int level)
{
	//set spawners
	t_minesToSpawn mines = minesToSpawn_getMax(level);
	unsigned int totalMines = (mines.floatingMine + mines.fireballMine 
							+ mines.magneticMine + mines.magneticFireballMine)* 7;

	t_spawner* spawner;

	for (unsigned int i = 0; i < totalMines; i++)
	{
		spawner = dynamicArray_AddItem(&world->spawners);
		spawner->location = getRandomLocation();
		spawner->size = BIG;
	}

	for (unsigned int i = 0; i < NB_MINE_SIZES - 1; i++)
		world->minesBySize[i] = minesToSpawn_create();
	world->minesBySize[NB_MINE_SIZES - 1] = minesToSpawn_getMax(world->currentPlayLevel);
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
	if (world->players.nbItems > 0)
		widgets_render(world, assets, 0);
	if (world->players.nbItems > 1)
		widgets_render(world, assets, 1);

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

			bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI, true);
			player->bInputs.shoot = false;
			player->lastShoot = SDL_GetTicks();
		}
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, i);
		mine_tick(mine, deltaTime);

		//newTarget
		for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->players, &j); j++)
		{
			bool newTarget = false;

			t_player* player = dynamicArray_GetItem(&world->players, j);
			if (mine->target != NULL)
			{
				float distance = vectorSquareLength(substractVectors(
								player->entity.ref.origin, mine->entity.ref.origin));
				float currentDistance = vectorSquareLength(substractVectors(
										mine->target->entity.ref.origin, mine->entity.ref.origin));
				if (currentDistance > distance)
					newTarget = true;
			}
			else 
				newTarget = true;	

			if (newTarget)
				mine->target = player;
		}
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

t_bullet* bullets_polygon_collision(t_dynamicArray* bullets, polygon* polygon, unsigned int* bulletID, bool isBullet)
{
	for (unsigned int k = 0; dynamicArray_GetValidItemIndex(bullets, &k); k++)
	{
		t_bullet* bullet = dynamicArray_GetItem(bullets, k);
		if (bullet->isBullet == isBullet && circle_polgyon_collision(&bullet->collision, polygon))
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

		//bullet collision
		unsigned int bulletID = 0;
		t_bullet* bullet = bullets_polygon_collision(&world->bullets, &mine->entity.worldCollider, &bulletID, true);
		
		if (bullet != NULL)
		{
			bullet_destroy(bullet);
			dynamicArray_RemoveItem(&world->bullets, bulletID);

			if (mine->throwFireballs)
			{
				vector2D bulletLocation = mine->entity.ref.origin;
				//bulletLocation = addVectors(bulletLocation, scaleVector(mine->entity.ref.unitI, 100.0 * 0.25 + 10));

				t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

				vector2D shootDirection = substractVectors(mine->target->entity.ref.origin, mine->entity.ref.origin);
				
				if (! floatIsNearlyEqual(vectorLength(shootDirection), 0, 0.001f))
					bullet_init(bullet, bulletLocation, mine->entity.velocity, unitVector(shootDirection), false);
			}

			if (mine->sizeType > SMALL)
			{
				//t_spawner* spawner;
				for (unsigned int i = 0; i < 2; i++)
				{
					unsigned int newSize = mine->sizeType - 1;
					unsigned int currentID = mine->throwFireballs + 2 * mine->followPlayer;
					if (currentID == 0)
						world->minesBySize[newSize].floatingMine++;
					if (currentID == 1)
						world->minesBySize[newSize].magneticMine++;
					if (currentID == 2)
						world->minesBySize[newSize].fireballMine++;
					if (currentID == 3)
						world->minesBySize[newSize].magneticFireballMine++;
					//spawner = dynamicArray_AddItem(&world->spawners);
					//spawner->location = getRandomLocation();
					//spawner->size = mine->sizeType - 1;
					//spawner->mineType = mine->throwFireballs + mine->followPlayer * 2;
					//spawner->delay = 1.f;
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

		//fireball collision
		unsigned int bulletID = 0;
		t_bullet* bullet = bullets_polygon_collision(&world->bullets, &player->entity.worldCollider, &bulletID, false);	
		if (bullet != NULL)
		{
			bullet_destroy(bullet);
			dynamicArray_RemoveItem(&world->bullets, bulletID);
			player_damages(player, &world->players, i);
		}

		for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->mines, &j); j++)
		{
			t_mine* mine = dynamicArray_GetItem(&world->mines, j);
			t_entity* entity = &mine->entity;
			
			if (polygon_polgyon_collision(&player->entity.worldCollider, &entity->worldCollider))
			{
				player_damages(player, &world->players, i);
			}
		}
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->bullets, &i); i++)
	{
		t_bullet* bullet1 = dynamicArray_GetItem(&world->bullets, i);
		for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->bullets, &j); j++)
		{
			if (i != j)
			{
				t_bullet* bullet2 = dynamicArray_GetItem(&world->bullets, j);
				if (bullet1->isBullet != bullet2->isBullet)
				{
					if (circle_circle_collision(bullet1->collision, bullet2->collision))
					{
						bullet_destroy(bullet1);
						dynamicArray_RemoveItem(&world->bullets, i);
						bullet_destroy(bullet2);
						dynamicArray_RemoveItem(&world->bullets, j);
					}
				}
			}
		}
	}
}

void world_GameOver(t_world* world, t_level* level, SDL_Renderer* renderer)
{
	unsigned int playerID = 0;
	bool isNotOver = dynamicArray_GetValidItemIndex(&world->players, &playerID);

	if (world->isPaused)
	{
		textbox_render(renderer, world->widgets.pauseTextBox);
	}
	else if (!isNotOver)
	{
		textbox_render(renderer, world->widgets.gameOverTextBox);
	}
}

unsigned int mines_readyToSpawn(t_minesToSpawn* mines)
{
	unsigned int mineType = 5;
	if (mines->floatingMine > 0)
	{
		mineType = 0;
		mines->floatingMine--;
	}
	else if (mines->fireballMine > 0)
	{
		mineType = 1;
		mines->fireballMine--;
	}
	else if (mines->magneticMine > 0)
	{
		mineType = 2;
		mines->magneticMine--;
	}
	else if (mines->magneticFireballMine > 0)
	{
		mineType = 3;
		mines->magneticFireballMine--;
	}
	return mineType;
}

void world_loop(t_assets* assets, float deltaTime, t_level* level)
{
    t_world* world = ((t_world*) (level->data));

	world->spawnDelay -= deltaTime;

	if (world->spawners.usedItems == 0 && world->mines.usedItems == 0)
	{
		newWorldLevel(world, world->currentPlayLevel);
		world->currentPlayLevel++;
		world->spawnDelay = 3.f;
	}

	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->spawners, &i); i++)
	{
		t_spawner* spawner = dynamicArray_GetItem(&world->spawners, i);

		if (world->spawnDelay < 0.f)
		{
			world->spawnDelay = 0.3f;

			unsigned int mineType = mines_readyToSpawn(&world->minesBySize[2]);
			E_SIZE size = BIG;
			if (mineType == 5)
			{
				mineType = mines_readyToSpawn(&world->minesBySize[1]);
				size = MEDIUM;
			}	
			if (mineType == 5)
			{
				mineType = mines_readyToSpawn(&world->minesBySize[0]);
				size = SMALL;
			}	
			if (mineType != 5)
			{
				t_mine* mine = dynamicArray_AddItem(&world->mines);
				mine_init(mine, mineType, spawner->location, size);
				dynamicArray_RemoveItem(&world->spawners, i);
			}
		}
		else
		{
			//draw spawner
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

	if (! world->isPaused)
	{
		world_tick(world, deltaTime);

		world_collisions(world);
	}

	world_GameOver(world, level, assets->render->renderer);
}
