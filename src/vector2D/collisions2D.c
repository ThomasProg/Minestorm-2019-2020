#include "collisions2D.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

bool point2D_point2D_collision(point2D point1, point2D point2, decimal delta)
{
	return vectorIsNearlyEqual(point1, point2, delta);
}

bool point2D_line_collision(point2D point1, line line1, decimal delta)
{
	vector2D relativePointLocation = substractVectors(point1, line1.point);
	return floatIsNearlyEqual
		(relativePointLocation.x / line1.direction.x, 
		 relativePointLocation.y / line1.direction.y, 
		 delta);
}

bool point2D_segment_collision(point2D point1, segment segment1, decimal delta)
{
	line line;
	line.direction = substractVectors(segment1.pointB, segment1.pointA);
	line.point = segment1.pointA;

	if (!point2D_line_collision(point1, line, delta))
	{	
		return false;
	}

	point2D* pointList = malloc(2 * sizeof(*pointList));
	pointList[0] = segment1.pointA;
	pointList[1] = segment1.pointB;
	axisAlignedRectangle rec = aabbRectangleGenerate(pointList, 2);
	free(pointList);
	return point2D_axisAlignedRectangle_collision(point1, rec);
}

bool point2D_circle_collision(point2D point1, circle circle1)
{
	return vectorLength(substractVectors(point1, circle1.center)) < circle1.length;
}

bool point2D_axisAlignedRectangle_collision(point2D point1, axisAlignedRectangle rec1)
{
	return (fabs(point1.x - rec1.center.x) < rec1.halfSize.x //x axis collision
		&& fabs(point1.y - rec1.center.y < rec1.halfSize.y)); //y axis collision
}

bool point2D_orientedRectangle_collision(point2D point1, orientedRectangle orientedRec1)
{
	//change point coordinates to rectangle referential
	point1.x -= orientedRec1.rectangle.center.x;
	point1.y -= orientedRec1.rectangle.center.y;
	point1 = rotateVector(point1, - orientedRec1.angle);

	return point2D_axisAlignedRectangle_collision(point1, orientedRec1.rectangle);
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

bool line_circle_collision(line line1, circle circle1)
{
	decimal dot = dotProduct(line1.direction, substractVectors(circle1.center, line1.point));
	point2D nearestPoint = addVectors(line1.point, scaleVector(line1.direction, dot));
	decimal distance = distancePointToPoint(nearestPoint, circle1.center);
	return distance < circle1.length;
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

bool segment_circle_collision(segment segment1, circle circle)
{
	line line = { unitVector(substractVectors(segment1.pointA, segment1.pointB)), segment1.pointA };

	if (!line_circle_collision(line, circle))
		return false;

	decimal d = distancePointToPoint(segment1.pointA, segment1.pointB);
	return (distancePointToPoint(segment1.pointA, circle.center) > d 
		|| (distancePointToPoint(segment1.pointB, circle.center) > d));
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

bool circle_circle_collision(circle circle1, circle circle2)
{
	return vectorLength(substractVectors(circle1.center, circle2.center)) 
			< circle1.length + circle2.length;
}

bool axisAlignedRectangle_axisAlignedRectangle_collision
	(axisAlignedRectangle rec1, axisAlignedRectangle rec2)
{
	return floatIsNearlyEqual(rec1.center.x, rec2.center.x, 
							  rec1.halfSize.x + rec2.halfSize.x) //x axis collision
		&& floatIsNearlyEqual(rec1.center.y, rec2.center.y, 
							  rec1.halfSize.y + rec2.halfSize.y); //y axis collision
}

////////////////////////////////////////////////
//////////////// SAT COLLISIONS //////////////// 
////////////////////////////////////////////////

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
	vector2D projectionAxis;
	range polygonProjection, circleProjection;

	//project on polygon's projection axis
	for (unsigned int i = 0; i < convexPolygon1.size; i++)
	{
		projectionAxis = normalVector(substractVectors(
			convexPolygon1.points[i], convexPolygon1.points[(i + 1) % convexPolygon1.size]));

		polygonProjection = projectPolygon(&convexPolygon1, &projectionAxis);
		circleProjection  = projectCircle(&circle1, &projectionAxis);

		if (!rangeIntersect(polygonProjection, circleProjection))
		{
			return false;
		}
	}

	//project on circle's projection axis
	point2D closestPolygonPoint = getClosestPointToPoint(circle1.center, convexPolygon1.points, convexPolygon1.size);

	projectionAxis = substractVectors(circle1.center, closestPolygonPoint);
	polygonProjection  = projectPolygon(&convexPolygon1, &projectionAxis);
	circleProjection = projectCircle(&circle1, &projectionAxis);

	if (!rangeIntersect(polygonProjection, circleProjection))
	{
		return false;
	}

	return true;
}

bool convexPolygon_convexPolygon_SAT(convexPolygon convexPolygon1, convexPolygon convexPolygon2)
{
	vector2D normal;
	range range1, range2;

	//for each segment of the 1st polygon
	for (unsigned int i = 0; i < convexPolygon1.size; i++)
	{
		normal = normalVector(substractVectors(
			convexPolygon1.points[i], convexPolygon1.points[(i + 1) % convexPolygon1.size]));
		//normal = unitVector(normal);

		range1 = projectPolygon(&convexPolygon1, &normal);
		range2 = projectPolygon(&convexPolygon2, &normal);

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
	if (! axisAlignedRectangle_axisAlignedRectangle_collision(convexPolygon1.aabb, convexPolygon2.aabb))
		return false;

	return (convexPolygon_convexPolygon_SAT(convexPolygon1, convexPolygon2)
		&& convexPolygon_convexPolygon_SAT(convexPolygon2, convexPolygon1));
}

bool circle_polgyon_collision(circle* circle1, polygon* polygon1)
{
	unsigned int i = 0;
	while (i < polygon1->nbConvexPolygons)
	{
		if (circle_convexPolygon_collision(*circle1, polygon1->convexPolygons[i]))
		{
			return true;
		}
		i++;
	}

	return false;
}

bool polygon_polgyon_collision(polygon* poly1, polygon* poly2)
{
	if (! axisAlignedRectangle_axisAlignedRectangle_collision(poly1->aabb, poly2->aabb))
		return false;

	//for optimization
	unsigned int i = 0;
	convexPolygon* convexPolygonList1 = poly1->convexPolygons;
	unsigned int size1 = poly1->nbConvexPolygons;
	convexPolygon* convexPolygonList2 = poly2->convexPolygons;
	unsigned int size2 = poly2->nbConvexPolygons;

	while (i < size1)
	{
		unsigned int j = 0;
		while (j < size2)
		{
			if (convexPolygon_convexPolygon_collision(convexPolygonList1[i], convexPolygonList2[j]))
			{
				return true;
			}
			j++;
		}
		i++;
	}
	return false;
}
