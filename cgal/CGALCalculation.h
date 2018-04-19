#ifndef VECTORCALCULATION_H
#define VECTORCALCULATION_H


#include "predefine.h"
#include "Types.h"
#include <iostream>


class CGALCalculation
{
public:
    static Vector_3 getUnitNormal(Vertex* va, Vertex* vb, Vertex* vc);
    static double getAngle(Vector_3& v1, Vector_3& v2);
    static double getAngle(Vector_2& v1, Vector_2& v2);

    static Vector_3 getVector(Vertex* va, Vertex* vb);

    static double getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3);
    static double getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc);

    static Vector_3 getCrossProduct(Vertex* va, Vertex* vb, Vertex* vc);

    static bool isIntersect_BBOX(Surface* , Surface*);
    static bool isAngleLowerThan(Vector_3& nv1, Vector_3& nv2, double);
    static bool isAngleLowerThan(Vertex* origin, Vertex* v1, Vertex* v2, double);

    static double getSquaredDistance(Vertex* v1, Vertex* v2);
    CGALCalculation(){}

    static int findNormalType6(Vector_3& nv);
    static Vector_3 normal_list6[6];
protected:

private:
};

#endif // VECTORCALCULATION_H
