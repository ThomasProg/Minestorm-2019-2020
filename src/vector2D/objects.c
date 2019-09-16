#include "objects.h"
#include <stdio.h>

//point2D coordonates are already in world dimensions
//return point2D relative to the referential
point2D worldToLocal_point2D(point2D point1, referential ref)
{
    point2D newPoint;
    newPoint.x = dotProduct(point1, ref.unitI);
    newPoint.y = dotProduct(point1, ref.unitJ);
    newPoint = substractVectors(newPoint, ref.origin);
    return newPoint;
}

//point2D coordonates are relative to the referential
//return point2D relative to world origin
point2D localToWorld_point2D(point2D point1, referential ref)
{
    point2D newPoint = addVectors(point1, ref.origin);
    newPoint = addVectors(scaleVector(ref.unitI, newPoint.x), scaleVector(ref.unitJ, newPoint.y));
    return newPoint;
}

vector2D worldToLocal_vector2D(vector2D vector1, referential ref)
{
    point2D newVector;
    newVector.x = dotProduct(vector1, ref.unitI);
    newVector.y = dotProduct(vector1, ref.unitJ);
    return newVector;
}

vector2D localToWorld_vector2D(vector2D vector1, referential ref)
{
    point2D newVector = addVectors(scaleVector(ref.unitI, vector1.x), scaleVector(ref.unitJ, vector1.y));
    return newVector;
}
