#ifndef _MENU_H_
#define _MENU_H_

#include "SDL_utilities/SDL_utilities.h"
#include "dynamicArray/dynamicArray.h"

#include "game.h"
#include "level.h"

typedef struct 
{
   
} t_menu;

t_menu* menu_create();
void menu_loop(t_assets* assets, float deltaTime, t_level* level);
void menu_destroy(t_menu* menu);

#endif
