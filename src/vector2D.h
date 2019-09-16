#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

typedef struct s_vector2D
{
	//float x, y; //to use to try fluidity
	int x, y;
} t_vector2D;

int vector2D_equal(t_vector2D* a, t_vector2D* b);

#endif
