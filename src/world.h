#ifndef _WORLD_H_
#define _WORLD_H_

#include "SDL_utilities/SDL_utilities.h"
#include "dynamicArray/dynamicArray.h"

#include "entities/mines_subtype/floating_mine.h"
#include "entities/mines_subtype/magnetic_mine.h"
#include "entities/mines_subtype/fireball_mine.h"

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

    t_textbox* scoreTextBox;
    t_textbox* lifeTextBox;
} t_widgets;

typedef struct 
{
    t_dynamicArray players;
    t_dynamicArray spawners;

    t_dynamicArray mines;
    t_dynamicArray bullets;

    bool isPaused;

    t_widgets widgets;
} t_world;


t_world* world_create(t_assets* assets);
void world_loop(t_assets* assets, float deltaTime, t_level* level);
void world_destroy(t_world* world);

void newWorldLevel(t_world* world, unsigned int level);

#endif