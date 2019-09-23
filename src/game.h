#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>

#include "SDL_utilities/SDL_utilities.h"
#include "level.h"

typedef struct 
{
    t_level level;
    t_assets* assets;
    bool run;
} game;

#endif
