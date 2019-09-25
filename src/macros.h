#ifndef _MACROS_H_
#define _MACROS_H_

#define WINDOW_SCALE 0.8

//window
#define WINDOW_SIZE_X 1500 * WINDOW_SCALE
#define WINDOW_SIZE_Y 800 * WINDOW_SCALE

#define START_LEVEL 4

//Entities Physics
#define AIR_FRICTION 600.f
#define SPEED_LIMIT  500.f
//a high value means less drift power
//0 = no drift
#define DRIFT_LEVEL 2.f 

//Player
#define NB_PLAYERS          1
#define PLAYER_LIFE         3
#define SHOOT_INTERVAL      200
#define TELEPORTATION_DELAY 0.2f
#define ANGULAR_SPEED       4.f
#define ACCELERATION        3000.f
#define ENTITY_MASS         3.f
#define SHIP_SIZE           0.35f * WINDOW_SCALE

//Mines stats
//#define FLOATING_MINE_VELOCITY {100.1f, 30.3f}
#define FLOATING_MINE_SPEED 80.f
#define MAGNETIC_MINE_SPEED 80.f
#define NB_MINE_SIZES       3
#define MINELAYER_SPAWNRATE 10 > 7//-1

#define SMALL_SPEED     1.5f;
#define MEDIUM_SPEED    1.3f;
#define BIG_SPEED       1.f;

#define SMALL_SIZE     0.4f;
#define MEDIUM_SIZE    0.7f;
#define BIG_SIZE       1.f;

//Bullets stats
#define BULLET_SPEED        250
#define BULLETS_SIZE        7.f * WINDOW_SCALE
#define BULLETS_PRECISION   15
#define BULLETS_TIME_ALIVE  3.5f

//Spawners
#define SPAWNER_SIZE  10
#define SPAWNER_COLOR 50, 50, 50, 0

//Maths
#define PI 3.14159265359f

#endif
