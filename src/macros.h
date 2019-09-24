#ifndef _MACROS_H_
#define _MACROS_H_

#define WINDOW_SCALE 0.7

//window
#define WINDOW_SIZE_X 1500 * WINDOW_SCALE
#define WINDOW_SIZE_Y 800 * WINDOW_SCALE

//Entities Physics
#define AIR_FRICTION 600.f
#define SPEED_LIMIT 500.f
//a high value means less drift power
//0 = no drift
#define DRIFT_LEVEL 2.f 

//Player
#define NB_PLAYERS 2
#define SHOOT_INTERVAL 400
#define ANGULAR_SPEED 4.f
#define ACCELERATION 3000.f
#define ENTITY_MASS 3.f
#define SHIP_SIZE 0.25f * WINDOW_SCALE

//Mines stats
#define FLOATING_MINE_VELOCITY {100.1f, 30.3f}
#define MAGNETIC_MINE_SPEED 125.f

//Bullets stats
#define BULLETS_SIZE 7.f * WINDOW_SCALE
#define BULLETS_PRECISION 15
#define BULLETS_TIME_ALIVE 2.f

//Maths
#define PI 3.14159265359f

#endif
