#ifndef VECTORCALCULATION_H
#define VECTORCALCULATION_H


#include "predefine.h"
#include "detail/cgal_config.h"
#include <iostream>

class Vertex;
class CGALCalculation
{
public:
    static double getAngle(Vector_3& v1, Vector_3& v2);

    static double getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3);
    static double getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc);

    static Vector_3 getCrossProduct(Vertex* va, Vertex* vb, Vertex* vc);
    static bool isAngleLowerThan(Vector_3& nv1, Vector_3& nv2, double);
    static bool isAngleLowerThan(Vertex* origin, Vertex* v1, Vertex* v2, double);

    CGALCalculation(){}

    static int findNormalType6(Vector_3& nv);
    static Vector_3 normal_list6[6];
protected:

private:
};

#endif // VECTORCALCULATION_H
