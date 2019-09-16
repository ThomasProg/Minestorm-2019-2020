#include "vector2D.h"
#include <math.h>
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
	decimal cosinus = cos(angle);
	decimal sinus = sin(angle);
	return (vector2D) { vec.x * cosinus - vec.y * sinus, vec.x * sinus + vec.y * cosinus };
}

vector2D rotateVector90(vector2D vec, decimal angle)
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
//bool inRange(decimal value, range range1)
//{
//	return (value > range1.min && value < range1.max);
//}
