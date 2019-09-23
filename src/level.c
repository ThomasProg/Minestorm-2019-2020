#include "level.h"

#include "world.h"
#include "menu.h"

void level_init(t_level* level, E_LEVEL levelID)
{
    level->levelID = levelID;
    level->nextLevel = E_NULL;

    switch (levelID)
    {
        case E_PLAY :
            level->data = world_create();
            break;
        case E_MENU :
		    level->data = menu_create();
		    break;
        case E_QUIT :
            break;
        case E_NULL :
            break;
    }
}

void level_destroy(t_level* level)
{
    switch (level->levelID)
    {
        case E_PLAY :
            world_destroy(level->data);
            break;
        case E_MENU :
		    menu_destroy(level->data);
            break;
        case E_QUIT :
            break;
        case E_NULL :
            break;
    }

    level->data = NULL;
    level->levelID = E_NULL;
    level->nextLevel = E_NULL;
}

void level_modify(t_level* level, E_LEVEL newLevelID)
{
    level_destroy(level);
    level_init(level, newLevelID);
}

void level_modifySafe(t_level* level, E_LEVEL newLevelID)
{
    level->nextLevel = newLevelID;
}

void level_tick(t_level* level, t_assets* assets, float deltaTime)
{
    switch (level->levelID)
    {
        case E_MENU :
            menu_loop(assets, deltaTime, level);
            break;
        case E_PLAY : 
            world_loop(assets, deltaTime, level);
            break;
        case E_QUIT :
            break;

        default : 
            break;
    }

    if (level->nextLevel != E_NULL)
    {
        level_modify(level, level->nextLevel);
        level->nextLevel = E_NULL;
    }
}