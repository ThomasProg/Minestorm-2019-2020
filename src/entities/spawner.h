#ifndef _SPAWNER_H_
#define _SPAWNER_H_

#include "vector2D/vector2D.h"
#include "mines.h"

typedef struct 
{
    E_SIZE size;
    float delay;
    //unsigned int mineID;
    vector2D location;

    int mineType;
} t_spawner;


#endif