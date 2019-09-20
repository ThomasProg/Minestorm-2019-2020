#include "level.h"

#include "world.h"

void level_init(level* level, E_LEVEL levelID)
{
    level->levelID = levelID;
    switch (levelID)
    {
        case E_PLAY :
            level->data = world_create();
            break;
        case E_MENU :

            break;
    }
}

void level_destroy(level* level)
{
    switch (level->levelID)
    {
        case E_PLAY :
            world_destroy(level->data);
            break;
        case E_MENU :

            break;
    }
}

void level_modify(level* level, E_LEVEL newLevelID)
{
    level_destroy(level);
    level_init(level, newLevelID);
}