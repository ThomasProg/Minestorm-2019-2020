#include "collisions2D.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE_OF_ARRAY(array) sizeof(array) / sizeof(array[0])

bool point2D_point2D_collision(point2D a, point2D b, decimal delta)
{
	return vectorIsNearlyEqual(a, b, delta);
}

bool point2D_line_collision(point2D point, line line, decimal delta)
{
	vector2D relativePointLocation = substractVectors(point, line.point);
	return floatIsNearlyEqual(relativePointLocation.x / line.direction.x, relativePointLocation.y / line.direction.y, delta);
}

bool point2D_segment_collision(point2D point, segment segment, decimal delta)
{
	line line;
	line.direction = substractVectors(segment.pointB, segment.pointA);
	line.point = segment.pointA;

	if (!point2D_line_collision(point, line, delta))
	{	
		printf("point/segment collision : not on line");
		return false;
	}

	//create rectangle around the segment
	/*vector2D min, max;
	//set x
	if (segment.pointA.x < segment.pointB.x)
	{
		min.x = segment.pointA.x;
		max.x = segment.pointB.x;
	}
	else
	{
		max.x = segment.pointA.x;
		min.x = segment.pointB.x;
	}
	//set y
	if (segment.pointA.y < segment.pointB.y)
	{
		min.y = segment.pointA.y;
		max.y = segment.pointB.y;
	}
	else
	{
		max.y = segment.pointA.y;
		min.y = segment.pointB.y;
	}*/
	//point2D points[4] = {segment.pointA, segment.pointB};
	point2D* pointList = malloc(2 * sizeof(*pointList));
	pointList[0] = segment.pointA;
	pointList[1] = segment.pointB;
	axisAlignedRectangle rec = aabbRectangleGenerate(pointList, 2);
	free(pointList);
	printf("1 : %f 2 : %f 3 : %f 4 : %f\n", rec.center.x, rec.center.y, rec.halfSize.x, rec.halfSize.y);
	return point2D_axisAlignedRectangle_collision(point, rec);
	//if (!vectorIsNearlyEqual(point, scaleVector(addVectors(segment.pointA, segment.pointB), 2), line.direction));
	//return (!(min.x < point.x && point.x < max.x && min.y < point.y && point.y < max.y)); // is point not in rectangle ?
}

bool point2D_circle_collision(point2D a, circle b)
{
	return vectorLength(substractVectors(a, b.center)) < b.length;
}

bool point2D_axisAlignedRectangle_collision(point2D a, axisAlignedRectangle b)
{
	return (fabs(a.x - b.center.x) < b.halfSize.x //x axis collision
		&& fabs(a.y - b.center.y < b.halfSize.y)); //y axis collision
}

bool point2D_orientedRectangle_collision(point2D a, orientedRectangle b)
{
	//change point coordinates to rectangle referential
	a.x -= b.rectangle.center.x;
	a.y -= b.rectangle.center.y;
	a = rotateVector(a, - b.angle);

	return point2D_axisAlignedRectangle_collision(a, b.rectangle);
}

bool point2D_convexPolygon_collision(point2D point1, convexPolygon convexPolygon1)
{
	if (convexPolygon1.points == NULL)
		return false;

	//TODO : if array size < 3//

	triangle triangle1;
	triangle1.a = convexPolygon1.points[0];

	for (unsigned int i = 2; i < convexPolygon1.size/*SIZE_OF_ARRAY(convexPolygon1.points)*/; i++)
	{
		triangle1.b = convexPolygon1.points[i];
		triangle1.c = convexPolygon1.points[i - 1];

		if (point2D_triangle_collision(point1, triangle1))
		{
			return true;
		}
	}
	return false;
}

bool point2D_triangle_collision(point2D point1, triangle triangle1)
{
	vector2D vec1 = substractVectors(triangle1.a,triangle1.b);
	vector2D vec2 = substractVectors(triangle1.b,triangle1.c);
	vector2D vec3 = substractVectors(triangle1.c,triangle1.a);

	if (dotProduct(point1, vec1) * dotProduct(triangle1.c, vec1) >= 0)
	{
		if (dotProduct(point1, vec2) * dotProduct(triangle1.a, vec2) >= 0)
		{
			if (dotProduct(point1, vec3) * dotProduct(triangle1.b, vec3) >= 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool line_line_collision(line line1, line line2, decimal delta)
{
	vector2D n1 = normalVector(line1.direction);
	decimal result = dotProduct(line2.direction, n1);
	if (!floatIsNearlyEqual(result, 0.0, delta))
	{
		return true;
	}
	else
	{
		result = dotProduct(substractVectors(line1.point, line2.point), n1);
		return !floatIsNearlyEqual(result, 0.0, delta);
	}
}

// bool line_circle_collision(line line, circle circle)
// {
// 	point2D circle_center = {circle.center.x, circle.center.y};
// 	decimal x1 = line.point.x;
// 	decimal y1 = line.point.y;
// 	decimal x2 = x1 + line.direction.x;
// 	decimal y2 = y1 + line.direction.y;

// 	decimal distance = abs((y2 - y1) * circle_center.x + (x2 - x1) * circle_center.y + x2*x1 - y2*x1) 
// 						/ sqrt(pow(y2-y1, 2) + pow(x2-x1,2));

// 	return distance <= circle.length;
// }

bool line_circle_collision(line line1, circle circle1)
{
	//substractVectors(circle1.center, line1.point)
	//decimal dot = dotProduct(normalVector(line1.direction), substractVectors(line1.point, circle1.center));
	decimal dot = dotProduct(line1.direction, substractVectors(circle1.center, line1.point));
	printf("dot product : %f\n", dot);
	point2D nearestPoint = addVectors(line1.point, scaleVector(line1.direction, dot));
	printf("nearest Point relative : x = %f y = %f\n", nearestPoint.x, nearestPoint.y);
	decimal distance = distancePointToPoint(nearestPoint, circle1.center);
	return distance < circle1.length;
	/*
	vector2D normal = normalVector(line1.direction);

	point2D Ab = addVectors(line1.point, line1.direction);
	assert(Ab.x - line1.point.x != 0); //division by 0

	decimal slopeAX = (Ab.y - line1.point.y) / (Ab.x - line1.point.x);
	decimal bAX = line1.point.y - slopeAX * line1.point.x;

	printf("x : %f y : %f\n", Ab.x - line1.point.x, Ab.y - line1.point.x);
	printf("x : %f b : %f\n", slopeAX, bAX);

	point2D Ob = addVectors(circle1.center, normal);
	decimal slopeOX = (Ob.y - circle1.center.y) / (Ob.x - circle1.center.x);
	decimal bOX = circle1.center.y - slopeOX * circle1.center.x;

	printf("x : %f b : %f\n", slopeOX, bOX);

	point2D closestPoint;
	closestPoint.x = (bOX - bAX) / (slopeAX - slopeOX);
	closestPoint.y = slopeAX * closestPoint.x + bAX;

	printf("x : %f y : %f\n", closestPoint.x, closestPoint.y);

	decimal distance = distancePointToPoint(closestPoint, circle1.center);
	return distance < circle1.length;
	*/
}

segment* axisAlignedRectangle_to_segments(axisAlignedRectangle rec)
{
	//get rectangle points
	point2D points[4];
	points[0] = (point2D) {rec.center.x - rec.halfSize.x, rec.center.y + rec.halfSize.y};
	points[1] = (point2D) {rec.center.x + rec.halfSize.x, rec.center.y + rec.halfSize.y};
	points[2] = (point2D) {rec.center.x + rec.halfSize.x, rec.center.y - rec.halfSize.y};
	points[3] = (point2D) {rec.center.x - rec.halfSize.x, rec.center.y - rec.halfSize.y};
	
	//get segments from points
	segment* segments = malloc(4 * sizeof(*segments));
	for (int i = 0; i < 4; i++)
		segments[i] = (segment) {points[i], points[(i+1) % 4]};

	return segments;
}

bool line_axisAlignedRectangle_collision(line line1, axisAlignedRectangle rectangle1)
{
	segment* segments = axisAlignedRectangle_to_segments(rectangle1);
	//if line cut one segment, then it cuts another one
	//so useless to calculate for the last one
	for (unsigned int i = 0; i < 3; i++) 
	{
		if (segment_line_collision(segments[i], line1))
		{
			free(segments);
			return true;
		}
	}
	free(segments);
	return false;
}

bool segment_line_collision(segment segment1, line line1)
{
		return ((dotProduct(segment1.pointA, line1.direction) *
	 dotProduct(segment1.pointB, line1.direction) < 0));
}

bool segment_segment_collision(segment segment1, segment segment2)
{
	line line1 = segmentToLine(segment1);
	line line2 = segmentToLine(segment2);

	if (!segment_line_collision(segment2, line1) || !segment_line_collision(segment1, line2))
		return false;
	else
	{
		if (dotProduct(normalVector(line1.direction), line2.direction) != 0)
			return true;
		else
		{
			decimal a1 = dotProduct(segment1.pointA, line1.direction);
			decimal b1 = dotProduct(segment1.pointB, line1.direction);
			range range1 = rangeCreate(a1, b1);

			decimal a2 = dotProduct(segment1.pointA, line1.direction);
			decimal b2 = dotProduct(segment1.pointB, line1.direction);
			range range2 = rangeCreate(a2, b2);

			return !(range2.max < range1.min && range1.max < range2.min); //idk if it's correct
		}
	}
}

bool segment_circle_collision(segment segment, circle circle)
{
	line line = { unitVector(substractVectors(segment.pointA, segment.pointB)), segment.pointA };

	if (!line_circle_collision(line, circle))
		return false;

	decimal d = distancePointToPoint(segment.pointA, segment.pointB);
	return (distancePointToPoint(segment.pointA, circle.center) > d || (distancePointToPoint(segment.pointB, circle.center) > d));
}

bool segment_axisAlignedRectangle_collision(segment segment1, axisAlignedRectangle rectangle1)
{
	if (point2D_axisAlignedRectangle_collision(segment1.pointA, rectangle1)
		|| point2D_axisAlignedRectangle_collision(segment1.pointB, rectangle1))
		{
			return true;
		}
	segment* segments = axisAlignedRectangle_to_segments(rectangle1);
	//if line cut one segment, then it cuts another one
	//so useless to calculate for the last one
	for (unsigned int i = 0; i < 3; i++) 
	{
		if (segment_segment_collision(segments[i], segment1))
		{
			free(segments);
			return true;
		}
	}
	free(segments);
	return false;
}



bool circle_circle_collision(circle a, circle b)
{
	return vectorLength(substractVectors(a.center, b.center)) < a.length + b.length;
}

bool axisAlignedRectangle_axisAlignedRectangle_collision(axisAlignedRectangle a, axisAlignedRectangle b)
{
	return floatIsNearlyEqual(a.center.x, b.center.x, a.halfSize.x + b.halfSize.x) //x axis collision
		&& floatIsNearlyEqual(a.center.y, b.center.y, a.halfSize.y + b.halfSize.y); //y axis collision
}

range projectPolygon(convexPolygon* convexPolygon1, vector2D* axis)
{
	range returned;
	decimal dotProd;
	returned.min = dotProduct(convexPolygon1->points[0], *axis);
	returned.max = returned.min;

	for (unsigned i = 0; i < convexPolygon1->size; i++)
	{
		dotProd = dotProduct(convexPolygon1->points[i], *axis);
		returned = rangeAddScalar(returned, dotProd);
	}
	return returned;
}

range projectCircle(circle* circle1, vector2D* axis)
{
	range returned;
	returned.min = dotProduct(circle1->center, *axis);
	returned.max = returned.min + circle1->length;
	returned.min -= circle1->length;

	return returned;
}

//SAT algorithm
bool circle_convexPolygon_collision(circle circle1, convexPolygon convexPolygon1)
{
	vector2D normal;
	range range1, range2;

	for (unsigned int i = 0; i < convexPolygon1.size; i++)
	{
		normal = normalVector(substractVectors(
			convexPolygon1.points[i], convexPolygon1.points[(i + 1) % convexPolygon1.size]));

		range1 = projectPolygon(&convexPolygon1, &normal);
		range2 = projectCircle(&circle1, &normal);

		if (!rangeIntersect(range1, range2))
		{
			return false;
		}
	}

	return true;
}

//SAT algorithm
bool convexPolygon_convexPolygon_collision(convexPolygon convexPolygon1, convexPolygon convexPolygon2)
{
	vector2D normal;
	range range1, range2;

	for (unsigned int i = 0; i < convexPolygon1.size; i++)
	{
		normal = normalVector(substractVectors(
			convexPolygon1.points[i], convexPolygon1.points[(i + 1) % convexPolygon1.size]));

		range1 = projectPolygon(&convexPolygon1, &normal);
		range2 = projectPolygon(&convexPolygon2, &normal);

		if (!rangeIntersect(range1, range2))
		{
			return false;
		}
	}

	for (unsigned int i = 0; i < convexPolygon2.size; i++)
	{
		normal = normalVector(substractVectors(
			convexPolygon2.points[i], convexPolygon2.points[(i + 1) % convexPolygon1.size]));

		range1 = projectPolygon(&convexPolygon1, &normal);
		range2 = projectPolygon(&convexPolygon2, &normal);

		if (!rangeIntersect(range1, range2))
		{
			return false;
		}
	}

	return true;
}
