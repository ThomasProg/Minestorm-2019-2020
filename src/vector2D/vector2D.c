#include "vector2D.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

vector2D nullVector()
{
	return (vector2D) { 0.f, 0.f };
}

decimal vectorSquareLength(vector2D vec)
{
	return (pow(vec.x, 2) + pow(vec.y, 2));
}

decimal vectorLength(vector2D vec)
{
	return sqrt(vectorSquareLength(vec));
}

vector2D unitVector(vector2D vec)
{
	decimal d = vectorLength(vec);
	assert(d != 0.f); //VECTOR OF NULL LENGTH
	//if (d != 0.f)
	//{
		return (vector2D){ vec.x / d, vec.y / d };
	//}
	//else
	//	assert();
		//return nullVector();
}

bool vectorIsNearlyEqual(vector2D u, vector2D v, decimal delta)
{
	return (fabs(u.x - v.x) < delta && fabs(u.y - v.y) < delta); // an change into floatIsNearlyEqual()
}

bool floatIsNearlyEqual(decimal a, decimal b, decimal delta)
{
	return fabs(a - b) < delta;
}

decimal distancePointToPoint(vector2D u, vector2D v)
{
	return vectorLength((vector2D) {u.x - v.x, u.y - v.y});
}

vector2D addVectors(vector2D u, vector2D v)
{
	return (vector2D) { u.x + v.x, u.y + v.y };
}

vector2D substractVectors(vector2D u, vector2D v)
{
	return (vector2D) { u.x - v.x, u.y - v.y };
}

vector2D scaleVector(vector2D vec, decimal scalar)
{
	return (vector2D) { vec.x * scalar, vec.y * scalar };
}

vector2D negateVector(vector2D vec)
{
	return (vector2D) { - vec.x, - vec.y };
}

vector2D rotateVector(vector2D vec, decimal angle)
{
	decimal cosinus = cosf(angle);
	decimal sinus = sinf(angle);
	return (vector2D) { vec.x * cosinus - vec.y * sinus, vec.x * sinus + vec.y * cosinus };
}

vector2D rotateVector90(vector2D vec)
{
	return (vector2D) { - vec.y, vec.x };
}

vector2D rotatePoint(point2D toRotate, point2D center, decimal angle)
{
	point2D relativeLoc = substractVectors(toRotate, center);

	relativeLoc = rotateVector(relativeLoc, angle);
	
	return addVectors(relativeLoc, center);
}

vector2D normalVector(vector2D vec)
{
	vector2D normalVec = {- vec.y, vec.x};
	return normalVec; 
}

decimal dotProduct(vector2D u, vector2D v)
{
	return u.x * v.x + u.y * v.y;
}

decimal vectorAngle(vector2D vector1)
{
	return atan2f(vector1.x, vector1.y);
}

decimal vectorsAngle(vector2D vector1, vector2D vector2)
{
	return acos(dotProduct(vector1, vector2) / vectorLength(vector1) / vectorLength(vector2));
}

line segmentToLine(segment segment1)
{
	line line1;
	line1.direction = substractVectors(segment1.pointA, segment1.pointB);
	line1.point = segment1.pointA;
	return line1;
}
 
range rangeCreate(decimal a, decimal b)
{
	range r;
	if (a < b)
	{
		r.min = a;
		r.max = b;
	}
	else
	{
		r.min = b;
		r.max = a;
	}
	return r;
}

range rangeAddScalar(range range1, decimal scalar)
{
	if (scalar > range1.max)
		range1.max = scalar;
	if (scalar < range1.min)
		range1.min = scalar;
	return range1;
}

bool inRange(decimal value, range range1)
{
	return (value > range1.min && value < range1.max);
}

bool rangeIntersect(range range1, range range2)
{
	return range1.max > range2.min && range2.max > range1.min;
}

axisAlignedRectangle aabbRectangleGenerate(point2D* points, unsigned int size)
{
	axisAlignedRectangle rect;

	vector2D min = points[0];
	vector2D max = min;
	for (int i = 1; i < size; i++)
	{
		if (points[i].x < min.x)
			min.x = points[i].x;
		if (points[i].y < min.y)
			min.y = points[i].y;

		if (points[i].x > max.x)
			max.x = points[i].x;
		if (points[i].y > max.y)
			max.y = points[i].y;			
	}

	rect.halfSize.x = (max.x - min.x) / 2;
	rect.halfSize.y = (max.y - min.y) / 2;
	rect.center.x = min.x + rect.halfSize.x;
	rect.center.y = min.y + rect.halfSize.y;
	return rect;
}

// axisAlignedRectangle circleToAxisAlignedRectangle(circle* circle1)
// {
// 	axisAlignedRectangle rect;

// 	rect.halfSize.x = circle1->length;
// 	rect.halfSize.y = circle1->length;
// 	rect.center = circle1->center;
// 	return rect;
// }

axisAlignedRectangle fuseAxisAlignedRectangles(axisAlignedRectangle a, axisAlignedRectangle b)
{
	axisAlignedRectangle returned; 

	point2D amin = substractVectors(a.center, a.halfSize);
	point2D amax = addVectors(a.center, a.halfSize); 

	point2D bmin = substractVectors(b.center, b.halfSize);
	point2D bmax = addVectors(b.center, b.halfSize); 

	range rangeX = rangeCreate(amin.x, amax.x);
	rangeX = rangeAddScalar(rangeX, bmin.x);
	rangeX = rangeAddScalar(rangeX, bmax.x);

	range rangeY = rangeCreate(amin.y, amax.y);
	rangeY = rangeAddScalar(rangeY, bmin.y);
	rangeY = rangeAddScalar(rangeY, bmax.y);

	returned.center.x = (rangeX.max + rangeX.min) / 2;
	returned.center.y = (rangeY.max + rangeY.min) / 2;

	returned.halfSize.x = (rangeX.max - rangeX.min) / 2;
	returned.halfSize.y = (rangeY.max - rangeY.min) / 2;

	return returned;
}

void polygon_free(polygon* polygon1)
{
	for (unsigned int i = 0; i < polygon1->nbConvexPolygons; i++)
		free(polygon1->convexPolygons[i].points);

	free(polygon1->convexPolygons);
	//free(polygon);
}

void polygon_aabb_generate(polygon* polygon1)
{
	convexPolygon* convex = &polygon1->convexPolygons[0];
    convex->aabb = aabbRectangleGenerate(convex->points, convex->size);

	polygon1->aabb = convex->aabb; 
	
	for (unsigned int i = 1; i < polygon1->nbConvexPolygons; i++)
	{
		convex = &polygon1->convexPolygons[i];
    	convex->aabb = aabbRectangleGenerate(convex->points, convex->size);
		polygon1->aabb = fuseAxisAlignedRectangles(polygon1->aabb, convex->aabb);
	}
}

point2D getClosestPointToPoint(point2D point1, point2D* points, unsigned int nbPoints)
{
	if (nbPoints == 0)
		return nullVector(); //assert

	point2D returnedPoint = points[0];
	float minLength       = distancePointToPoint(point1, returnedPoint);
	float currentLength;

	for (unsigned int i = 1; i < nbPoints; i++)
	{
		currentLength = distancePointToPoint(point1, points[i]);
		if (currentLength < minLength)
		{
			minLength = currentLength;
			returnedPoint = points[i];
		}
	}

	return returnedPoint;
}