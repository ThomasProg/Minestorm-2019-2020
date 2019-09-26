#ifndef _MINES_H_
#define _MINES_H_

#include <entities/entities.h>
#include "entities/player.h"

typedef enum 
{
    SMALL = 0,
    MEDIUM,
    BIG
} E_SIZE;

typedef struct s_mine
{
    bool followPlayer;
    bool throwFireballs;
    bool isMineLayer;
    float mineLayerDelay;

    E_SIZE sizeType;
    float size; 
    unsigned int givenScore;
    
    t_entity entity;
    t_player* target;

} t_mine;

void mine_init            (t_mine* mine, unsigned int type, 
                          vector2D location, E_SIZE sizeType);
void floatingMine_init        (t_mine* mine, E_SIZE sizeType);
void fireballMine_init        (t_mine* mine, E_SIZE sizeType);
void magneticMine_init        (t_mine* mine, E_SIZE sizeType);
void magneticFireballMine_init(t_mine* mine, E_SIZE sizeType);

void mine_destroy   (t_mine* mine);
void mine_newTarget (t_dynamicArray* players, t_mine* mine);
void mine_render    (t_mine* mine, t_render* render, bool renderDebug);
void mine_tick      (t_mine* mine, float deltaTime);

void mine_collisionBox_init(polygon* collision, 
    unsigned int nbPolygons, float size1, float size2);

void mine_spawn(void* mine, unsigned int type, vector2D spawner);

vector2D getShortestPath         (vector2D a, vector2D b);
void magneticMine_searchForPlayer(t_mine* mine, float deltaTime);

void throwFireball(t_mine* mine, t_dynamicArray* bullets);

#endif
