#ifndef _MENU_H_
#define _MENU_H_

#include "SDL_utilities/SDL_utilities.h"
#include "dynamicArray/dynamicArray.h"

#include "widgets/button.h"
#include "widgets/textbox.h"

#include "game.h"
#include "level.h"

typedef struct 
{
    SDL_Rect mouseLocation;
    //t_dynamicArray buttons;
    //t_dynamicArray textboxes;
    t_button* button;
    t_textbox* textbox;
} t_menu;

t_menu* menu_create(t_assets* assets);
void menu_loop(t_assets* assets, float deltaTime, t_level* level);
void menu_destroy(t_menu* menu);

#endif
