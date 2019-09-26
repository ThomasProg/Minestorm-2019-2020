#ifndef _WORLD_H_
#define _WORLD_H_

#include "SDL_utilities/SDL_utilities.h"
#include "dynamicArray/dynamicArray.h"

#include "entities/mines.h"
#include "entities/spawner.h"
#include "entities/player.h"

#include "widgets/button.h"
#include "widgets/textbox.h"

#include "game.h"
#include "level.h"

typedef struct 
{
    t_button* menuButton;
    t_button* pauseButton;

    t_textbox* scoreTextBox1;
    t_textbox* lifeTextBox1;

    t_textbox* scoreTextBox2;
    t_textbox* lifeTextBox2;

    t_textbox* gameOverTextBox;
    t_textbox* pauseTextBox;

    t_textbox* commandesTextBox;
} t_widgets;

typedef struct 
{
    unsigned int floatingMine;
    unsigned int fireballMine;
    unsigned int magneticMine;
    unsigned int magneticFireballMine;
} t_minesToSpawn;

typedef struct s_world
{
    t_dynamicArray players;
    t_dynamicArray spawners;

    t_dynamicArray mines;
    t_dynamicArray bullets;

    bool isPaused;
    bool debugMode;

    t_widgets widgets;
    unsigned int currentPlayLevel;
    t_minesToSpawn* minesBySize;
    float spawnDelay;
} t_world;


t_world* world_create           (t_assets* assets, unsigned int nbPlayers);
void world_loop                 (t_assets* assets, float deltaTime, t_level* level);
void world_destroy              (t_world* world);
void world_render               (t_world* world, t_assets* assets);
void world_inputs               (t_level* level, t_world* world);
void world_keydownGlobalInputs  (int key, t_level* level, t_world* world);        

void world_tick  (t_world* world, float deltaTime);
void players_tick(t_world* world, float deltaTime);
void mines_tick  (t_world* world, float deltaTime);
void bullets_tick(t_world* world, float deltaTime);

void newWorldLevel(t_world* world, unsigned int level);

t_minesToSpawn minesToSpawn_create();
t_minesToSpawn minesToSpawn_getMax(unsigned int lvl);

void widgets_init(t_world* world, t_assets* assets);
void widgets_render(t_world* world, t_assets* assets, unsigned int playerID);
void widgets_destroy(t_widgets* widgets);

void newWorldLevel(t_world* world, unsigned int level);

//Collisions are happening between different elements of the world, that is why they are here.
//In future versions, they could move to a new file "world_collisions.c", or something similar
t_bullet* bullets_polygon_collision(t_dynamicArray* bullets, polygon* polygon, 
									    unsigned int* bulletID, bool isBullet);
void mines_collisions               (t_world* world);
void mine_addNewSpawningMine        (t_mine* mine, t_world* world);
void player_fireball_collisions     (t_player* player, unsigned int playerID,
								        t_dynamicArray* players, t_dynamicArray* bullets);
void player_mine_collisions         (t_player* player, unsigned int playerID,
							            t_dynamicArray* players, t_dynamicArray* mines);
void players_collisions             (t_world* world);
void bullets_collisions             (t_world* world);
void world_collisions               (t_world* world);

void         world_GameOver(t_world* world, t_level* level, SDL_Renderer* renderer);
unsigned int mines_readyToSpawn(t_minesToSpawn* mines);

//start newLevel if it is time
bool world_newLevelCheck(t_dynamicArray* spawners, t_dynamicArray* mines, unsigned int currentPlayLevel);
void world_levelCheck   (t_world* world);

void spawnMineFromSpawners(t_world* world, t_assets* assets, float deltaTime);

#endif