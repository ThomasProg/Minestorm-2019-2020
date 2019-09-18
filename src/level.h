#ifndef _LEVEL_H_
#define _LEVEL_H_

//TODO : sub gamestate for each level (pause, quit...)
/*
typedef enum 
{
    E_PAUSE,
    E_QUIT
} E_GAMESTATE;
*/

typedef enum 
{
    E_MENU = 0,
    E_PLAY,
} E_LEVEL;

typedef struct
{
    void* data;
    E_LEVEL levelID;
} level;

void level_init(level* level, E_LEVEL levelID);
void level_destroy(level* level);

void level_modify(level* level, E_LEVEL newLevelID);

#endif