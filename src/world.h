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

typedef struct 
{
    t_dynamicArray players;
    t_dynamicArray spawners;

    t_dynamicArray mines;
    t_dynamicArray bullets;

    bool isPaused;
    bool debugMode;

    t_widgets widgets;
    // t_minesToSpawn bigMinesToSpawn;
    // t_minesToSpawn mediumMinesToSpawn;
    // t_minesToSpawn smallMinesToSpawn;
    unsigned int currentPlayLevel;
    t_minesToSpawn* minesBySize;
    float spawnDelay;
} t_world;


t_world* world_create(t_assets* assets, unsigned int nbPlayers);
void world_loop(t_assets* assets, float deltaTime, t_level* level);
void world_destroy(t_world* world);

void newWorldLevel(t_world* world, unsigned int level);

#endif