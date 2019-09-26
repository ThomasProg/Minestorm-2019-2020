#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

#include <stdbool.h>
#define decimal float

typedef struct 
{
	decimal x, y;
} vector2D;

typedef vector2D point2D;

typedef struct
{
	vector2D direction;
	point2D point;
} line;

typedef struct
{
	point2D pointA;
	point2D pointB;
} segment;

typedef struct
{
	point2D center;
	vector2D halfSize; //half height, half width 
} axisAlignedRectangle;

typedef struct
{
	axisAlignedRectangle rectangle;
	decimal angle;
} orientedRectangle;

typedef struct
{
	point2D center;
	decimal length;
	//axisAlignedRectangle aabb;
} circle;

typedef struct
{
	point2D* points;
	unsigned int size;
	
	axisAlignedRectangle aabb;
} convexPolygon;

typedef struct 
{
	convexPolygon* convexPolygons;
	unsigned int nbConvexPolygons;

	axisAlignedRectangle aabb;
} polygon;

typedef struct 
{
	point2D a, b, c;
} triangle;


typedef struct
{
	decimal min, max;
} range;

vector2D nullVector();

decimal vectorSquareLength(vector2D vec);

decimal vectorLength(vector2D vec);

vector2D unitVector(vector2D vec);

bool vectorIsNearlyEqual(vector2D u, vector2D v, decimal delta);

bool floatIsNearlyEqual(decimal a, decimal b, decimal delta);

decimal distancePointToPoint(vector2D u, vector2D v);

vector2D addVectors(vector2D u, vector2D v);

vector2D substractVectors(vector2D u, vector2D v);

vector2D scaleVector(vector2D vec, decimal scalar);

vector2D negateVector(vector2D vec);

vector2D rotateVector(vector2D vec, decimal angle);

vector2D rotateVector90  (vector2D vec);
vector2D unrotateVector90(vector2D vec);

vector2D rotatePoint(point2D toRotate, point2D center, decimal angle);

vector2D normalVector(vector2D vec);

decimal dotProduct(vector2D u, vector2D v);

line segmentToLine(segment segment1);

decimal vectorsAngle(vector2D vector1, vector2D vector2);

decimal vectorAngle(vector2D vector1);

range rangeCreate(decimal a, decimal b);
range rangeAddScalar(range range1, decimal scalar);
bool rangeIntersect(range range1, range range2);

axisAlignedRectangle aabbRectangleGenerate(point2D* points, unsigned int size);
axisAlignedRectangle fuseAxisAlignedRectangles(axisAlignedRectangle a, axisAlignedRectangle b);

void polygon_free(polygon* polygon);
void polygon_aabb_generate(polygon* polygon1);

point2D getClosestPointToPoint(point2D point1, point2D* points, unsigned int nbPoints);

#endif
