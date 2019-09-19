#ifndef _MACROS_H_
#define _MACROS_H_

//window
#define WINDOW_SIZE_X 1200
#define WINDOW_SIZE_Y 800

//Entities Physics
#define AIR_FRICTION 600.f
#define SPEED_LIMIT 500.f
//a high value means less drift power
//0 = no drift
#define DRIFT_LEVEL 2.f 

//Player Physics
#define ANGULAR_SPEED 4.f
#define ACCELERATION 3000.f
#define ENTITY_MASS 3.f
#define SHIP_SIZE 0.25f

//Mines stats
#define FLOATING_MINE_VELOCITY {100.1f, 30.3f}
#define MAGNETIC_MINE_SPEED 1.25f

//Bullets stats
#define BULLETS_SIZE 7.f
#define BULLETS_PRECISION 15.f
#define BULLETS_TIME_ALIVE 2.f

//Maths
#define PI 3.14159265359f

#endif