#pragma once

#include "vector2D.h"


#include "collisions2D.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define SIZE_OF_ARRAY(array) sizeof(array) / sizeof(array[0])

bool point2D_point2D_collision(point2D a, point2D b, decimal delta);
bool point2D_line_collision(point2D point, line line, decimal delta);
bool point2D_segment_collision(point2D point, segment segment, decimal delta);
bool point2D_circle_collision(point2D a, circle b);
bool point2D_axisAlignedRectangle_collision(point2D a, axisAlignedRectangle b);
bool point2D_orientedRectangle_collision(point2D a, orientedRectangle b);
bool point2D_triangle_collision(point2D point1, triangle triangle1);
bool point2D_convexPolygon_collision(point2D point1, convexPolygon convexPolygon1);
bool point2D_triangle_collision(point2D point1, triangle triangle1);

bool line_line_collision(line line1, line line2, decimal delta);
bool line_circle_collision(line line1, circle circle1);
bool line_axisAlignedRectangle_collision(line line1, axisAlignedRectangle rectangle1);

bool segment_line_collision(segment segment1, line line1);
bool segment_segment_collision(segment segment1, segment segment2);
bool segment_circle_collision(segment segment, circle circle);

bool circle_circle_collision(circle a, circle b);
bool circle_convexPolygon_collision(circle circle1, convexPolygon convexPolygon1);

bool axisAlignedRectangle_axisAlignedRectangle_collision(axisAlignedRectangle a, axisAlignedRectangle b);

bool convexPolygon_convexPolygon_collision(convexPolygon convexPolygon1, convexPolygon convexPolygon2);

bool circle_polgyon_collision(circle* circle1, polygon* polygon1);
bool polygon_polgyon_collision(polygon* poly1, polygon* poly2);