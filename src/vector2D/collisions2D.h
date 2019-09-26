#pragma once

#include "vector2D.h"


#include "collisions2D.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

//arguments contain a number to tell that they are instances

bool point2D_point2D_collision             (point2D point1, point2D point2,  decimal delta);
bool point2D_line_collision                (point2D point1, line line,       decimal delta);
bool point2D_segment_collision             (point2D point1, segment segment1, decimal delta);
bool point2D_circle_collision              (point2D point1, circle circle1);
bool point2D_axisAlignedRectangle_collision(point2D point1, axisAlignedRectangle rec1);
bool point2D_orientedRectangle_collision   (point2D point1, orientedRectangle orientedRec1);
bool point2D_triangle_collision            (point2D point1, triangle triangle1);
bool point2D_convexPolygon_collision       (point2D point1, convexPolygon convexPolygon1);
bool point2D_triangle_collision            (point2D point1, triangle triangle1);

bool line_line_collision  (line line1, line line2, decimal delta);
bool line_circle_collision(line line1, circle circle1);
bool line_axisAlignedRectangle_collision
    (line line1, axisAlignedRectangle rectangle1);

bool segment_line_collision   (segment segment1, line line1);
bool segment_segment_collision(segment segment1, segment segment2);
bool segment_circle_collision (segment segment1, circle circle1);

bool axisAlignedRectangle_axisAlignedRectangle_collision
    (axisAlignedRectangle rec1, axisAlignedRectangle rec2);

////////////////////////////////////////////////
//////////////// SAT COLLISIONS //////////////// 
////////////////////////////////////////////////

bool circle_circle_collision(circle circle1, circle circle2);
bool circle_convexPolygon_collision(circle circle1, convexPolygon convexPolygon1);

bool convexPolygon_convexPolygon_collision
    (convexPolygon convexPolygon1, convexPolygon convexPolygon2);
//substep of convexPolygon_convexPolygon_collision ; 
//test only with the normals of the segments of one convexPolygon
bool convexPolygon_convexPolygon_SAT
    (convexPolygon convexPolygon1, convexPolygon convexPolygon2);

bool circle_polgyon_collision (circle* circle1, polygon* polygon1);
bool polygon_polgyon_collision(polygon* poly1, polygon* poly2);

range projectPolygon(convexPolygon* convexPolygon1, vector2D* axis);
range projectCircle (circle* circle1, vector2D* axis);