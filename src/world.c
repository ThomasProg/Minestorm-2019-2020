#include <assert.h>
#include <string.h>

#include "world.h"
#include "entities/entities.h"
#include "entities/mines_subtype/minelayer.h"
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

	while (lvl > 0)
	{
		returned.floatingMine++;
		if (returned.floatingMine == returned.fireballMine + 2)
		{
			returned.floatingMine = 0;
			returned.fireballMine++;
			if (returned.fireballMine == returned.magneticMine + 2)
			{
				returned.fireballMine = 0;
				returned.magneticMine++;
				if (returned.magneticMine == returned.magneticFireballMine + 2)
				{
					returned.magneticMine = 0;
					returned.magneticFireballMine++;
				}
			}
		}
		lvl --;
	}

	return returned;
}

void widgets_init(t_world* world, t_assets* assets)
{
	TTF_Font* font 		   = font_get(assets->font, 0);
	SDL_Rect rectScore     = {150, 0, 500, 500};
	SDL_Rect rectLife      = {150, 40, 500, 500};
	SDL_Rect rectGameOver  = {WINDOW_SIZE_X/2, 150, 500, 500};
	SDL_Rect rectCommandes = {WINDOW_SIZE_X/2, 550, 500, 500};

	world->widgets.scoreTextBox1 = textbox_create("SCORE : ", rectScore, font);
	world->widgets.lifeTextBox1  = textbox_create("LIFE : 3", rectLife, font);

	rectScore.x = WINDOW_SIZE_X - 150;  
	rectLife.x  = WINDOW_SIZE_X - 150;

	world->widgets.scoreTextBox2    = textbox_create("SCORE : ", rectScore, font);
	world->widgets.lifeTextBox2     = textbox_create("LIFE : 3", rectLife, font);
	world->widgets.gameOverTextBox  = textbox_create("GAME OVER\n Click on escape to return to the menu", rectGameOver, font);
	world->widgets.pauseTextBox     = textbox_create("PAUSE\n Press SPACE to resume", rectGameOver, font);
	world->widgets.commandesTextBox = textbox_create("SPACE to pause \n ESCAPE to go back to menu\n", rectCommandes, font);
}

t_world* world_create(t_assets* assets, unsigned int nbPlayers)
{
    t_world* world = malloc(sizeof(t_world));

	world->currentPlayLevel  = START_LEVEL;
	world->minesBySize = malloc(NB_MINE_SIZES * sizeof(t_minesToSpawn));

    world->spawnDelay   = 3.f;

	widgets_init(world, assets);

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

    return world;
}

void widgets_destroy(t_widgets* widgets)
{
	textbox_destroy(widgets->gameOverTextBox);
	free(widgets->gameOverTextBox);
	textbox_destroy(widgets->pauseTextBox);
	free(widgets->pauseTextBox);
	textbox_destroy(widgets->commandesTextBox);
	free(widgets->commandesTextBox);

	textbox_destroy(widgets->lifeTextBox1);
	free(widgets->lifeTextBox1);
	textbox_destroy(widgets->scoreTextBox1);
	free(widgets->scoreTextBox1);

	textbox_destroy(widgets->lifeTextBox2);
	free(widgets->lifeTextBox2);
	textbox_destroy(widgets->scoreTextBox2);
	free(widgets->scoreTextBox2);
}

void world_destroy(t_world* world)
{
	widgets_destroy(&world->widgets);

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

	textbox_render(assets->render->renderer, world->widgets.commandesTextBox);
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

void players_tick(t_world* world, float deltaTime)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);
		player_tick(player, deltaTime);

		if (player->bInputs.shoot && SDL_GetTicks() - player->lastShoot > SHOOT_INTERVAL)
		{
			vector2D bulletLocation = player->entity.ref.origin;
			bulletLocation = addVectors(bulletLocation, scaleVector(player->entity.ref.unitI, SHIP_SIZE * 4 * 100.0 * 0.25 + 10));

			t_bullet* bullet = dynamicArray_AddItem(&world->bullets);

			bullet_init(bullet, bulletLocation, player->entity.velocity, player->entity.ref.unitI, true);
			bullet->playerOwnerID = i;
			player->lastShoot = SDL_GetTicks();
		}
	}
}

void mines_tick(t_world* world, float deltaTime)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->mines, &i); i++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, i);
		mine_tick(mine, deltaTime);

		mine_newTarget(&world->players, mine);

		//minelayer
		if (mine->isMineLayer)
		{
			minelayer_tick(world, mine, i, deltaTime);
		}
	}
}

void bullets_tick(t_world* world, float deltaTime)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->bullets, &i); i++)
	{
		t_bullet* bullet = dynamicArray_GetItem(&world->bullets, i);
		bullet_tick(bullet, deltaTime);

		bullet = dynamicArray_GetItem(&world->bullets, i);

		bullet_tick(bullet, deltaTime);
		if (bullet->timeAlive > BULLETS_TIME_ALIVE)
		{
			dynamicArray_RemoveItem(&world->bullets, i);
		}
	}
}

void world_tick(t_world* world, float deltaTime)
{
	players_tick(world, deltaTime);
	mines_tick  (world, deltaTime);
	bullets_tick(world, deltaTime);
}

void world_keydownGlobalInputs(int key, t_level* level, t_world* world)
{
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
}

void world_inputs(t_level* level, t_world* world)
{
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN) //press
		{
			int key = event.key.keysym.sym;
			world_keydownGlobalInputs(key, level, world);

			for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
				player_input_start(dynamicArray_GetItem(&world->players, i), key, true);
		}
		if (event.type == SDL_KEYUP) //release
		{
			int key = event.key.keysym.sym;

			for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
				player_input_start(dynamicArray_GetItem(&world->players, i), key, false);
		}
		if (event.type == SDL_QUIT)
			level_modifySafe(level, E_QUIT);
	}
}

t_bullet* bullets_polygon_collision(t_dynamicArray* bullets, polygon* polygon, 
									unsigned int* bulletID, bool isBullet)
{
	for (unsigned int k = 0; dynamicArray_GetValidItemIndex(bullets, &k); k++)
	{
		t_bullet* bullet = dynamicArray_GetItem(bullets, k);
		if ((bullet->isBullet == isBullet || !isBullet) && circle_polgyon_collision(&bullet->collision, polygon))
		{
			if (bulletID != NULL)
				*bulletID = k;

			return bullet;
		}
	}

	return NULL;
}

void mine_addNewSpawningMine(t_mine* mine, t_world* world)
{
	if (mine->sizeType > SMALL && !mine->isMineLayer)
	{
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
		}
	}
}

void mines_collisions(t_world* world)
{
	for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->mines, &j); j++)
	{
		t_mine* mine = dynamicArray_GetItem(&world->mines, j);

		//bullet collision
		unsigned int bulletID = 0;
		t_bullet* bullet = bullets_polygon_collision(&world->bullets, &mine->entity.worldCollider, &bulletID, true);
		
		if (bullet != NULL)
		{
			dynamicArray_RemoveItem(&world->bullets, bulletID);

			if (mine->throwFireballs)
				throwFireball(mine, &world->bullets);		

			mine_addNewSpawningMine(mine, world);

			t_player* player = dynamicArray_GetItem(&world->players, bullet->playerOwnerID);//TODO : score to both player
			player->score += mine->givenScore;

			mine_destroy(mine);

			dynamicArray_RemoveItem(&world->mines, j);
		}
	}
}

void player_fireball_collisions(t_player* player, unsigned int playerID,
								t_dynamicArray* players, t_dynamicArray* bullets)
{
	unsigned int bulletID = 0;
	t_bullet* bullet = bullets_polygon_collision(bullets, &player->entity.worldCollider, &bulletID, false);	
	if (bullet != NULL)
	{
		dynamicArray_RemoveItem(bullets, bulletID);
		player_damages(player, players, playerID);
	}
}

void player_mine_collisions(t_player* player, unsigned int playerID,
							t_dynamicArray* players, t_dynamicArray* mines)
{
	//mine collision
	for (unsigned int j = 0; dynamicArray_GetValidItemIndex(mines, &j); j++)
	{
		t_mine* mine = dynamicArray_GetItem(mines, j);

		if (polygon_polgyon_collision(&player->entity.worldCollider, &mine->entity.worldCollider))
		{
			player_damages(player, players, playerID);
		}
	}	
}

void players_collisions(t_world* world)
{
	//for each player
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->players, &i); i++)
	{
		t_player* player = dynamicArray_GetItem(&world->players, i);

		player_fireball_collisions	(player, i, &world->players, &world->bullets);
		player_mine_collisions		(player, i, &world->players, &world->mines);
	}
}

void bullets_collisions(t_world* world)
{
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->bullets, &i); i++)
	{
		t_bullet* bullet1 = dynamicArray_GetItem(&world->bullets, i);
		for (unsigned int j = 0; dynamicArray_GetValidItemIndex(&world->bullets, &j); j++)
		{
			if (i != j) //if is not the same bullet
			{
				t_bullet* bullet2 = dynamicArray_GetItem(&world->bullets, j);
				if (bullet1->isBullet != bullet2->isBullet)
				{
					if (circle_circle_collision(bullet1->collision, bullet2->collision))
					{
						dynamicArray_RemoveItem(&world->bullets, i);
						dynamicArray_RemoveItem(&world->bullets, j);
					}
				}
			}
		}
	}
}

void world_collisions(t_world* world)
{
	mines_collisions  (world);
	players_collisions(world);
	bullets_collisions(world);
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

bool world_newLevelCheck(t_dynamicArray* spawners, t_dynamicArray* mines, unsigned int currentPlayLevel)
{
	if (spawners->usedItems == 0 && mines->usedItems == 0)
	{
		if (currentPlayLevel > START_LEVEL && rand() % MINELAYER_SPAWNRATE)
		{
			minelayer_create(mines);
		}
		else
		{
			return true;
		}
	}
	return false;
}

void world_levelCheck(t_world* world)
{
	if (world_newLevelCheck(&world->spawners, &world->mines, world->currentPlayLevel))
	{
		newWorldLevel(world, world->currentPlayLevel);
		world->currentPlayLevel++;
		world->spawnDelay = 3.f;
	}
}

void spawnMineFromSpawners(t_world* world, t_assets* assets, float deltaTime)
{
	world->spawnDelay -= deltaTime;
	for (unsigned int i = 0; dynamicArray_GetValidItemIndex(&world->spawners, &i); i++)
	{
		t_spawner* spawner = dynamicArray_GetItem(&world->spawners, i);
		if (world->spawnDelay < 0.f)
		{
			world->spawnDelay 	  = 0.3f;
			unsigned int mineType = mines_readyToSpawn(&world->minesBySize[2]);
			E_SIZE size 		  = BIG;
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
			spawner_render(assets->render->renderer, spawner);
	 }
}

void world_loop(t_assets* assets, float deltaTime, t_level* level)
{
    t_world* world = ((t_world*) (level->data));

	world_levelCheck(world);
	spawnMineFromSpawners(world, assets, deltaTime);

	world_render(world, assets);
	world_inputs(level, world);

	if (! world->isPaused)
	{
		world_tick(world, deltaTime);

		world_collisions(world);
	}

	world_GameOver(world, level, assets->render->renderer);
}
