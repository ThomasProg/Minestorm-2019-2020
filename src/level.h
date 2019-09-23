#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <stdbool.h>

#include "SDL_utilities/SDL_utilities.h"

typedef enum 
{
    E_NULL = -2,
    E_QUIT = -1,
    E_MENU = 0,
    E_PLAY,
} E_LEVEL;

typedef struct
{
    void* data;
    E_LEVEL levelID;
    E_LEVEL nextLevel;
} t_level;

void level_init(t_level* level, E_LEVEL levelID, t_assets* assets);
void level_destroy(t_level* level);

//destroy current level then init the selected one
void level_modify(t_level* level, E_LEVEL newLevelID, t_assets* assets);

//run level_modify at the end of the tick
//allow the dev to modify the level while in a level
//to prevent segmentation fault while trying to access to the level
void level_modifySafe(t_level* level, E_LEVEL newLevelID);

void level_tick(t_level* level, t_assets* assets, float deltaTime);

#endif
