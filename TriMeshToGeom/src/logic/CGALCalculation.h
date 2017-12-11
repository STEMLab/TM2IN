#ifndef VECTORCALCULATION_H
#define VECTORCALCULATION_H


#include "predefine.h"

#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>
#include <CGAL/Point_3.h>
#include <CGAL/Point_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Kernel/global_functions.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/enum.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/intersections.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Aff_transformation_3<Kernel> Transformation;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Triangle_3 Triangle_3;
typedef Kernel::Line_3 Line_3;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Intersect_2 Intersect_2;

class CGALCalculation
{
public:
    static Vector_3 getUnitNormal(Vertex* va, Vertex* vb, Vertex* vc);
    static double getAngle(Vector_3& v1, Vector_3& v2);
    static Vector_3 getVector(Vertex* va, Vertex* vb);

    static double getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3);
    static double getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc);

    static Vector_3 getCrossProduct(Vertex* va, Vertex* vb, Vertex* vc);

    static bool isIntersect2D(Segment* seg1, Segment* seg2);
    static Point_2 getIntersection2D(Segment* seg1, Segment* seg2);
    static bool isIntersect_BBOX(Surface* , Surface*);

    static int findNormalType27(Vector_3& nv);
    static int findNormalType18(Vector_3& nv);
    static int findNormalType10(Vector_3& nv);
    static int findNormalType6(Vector_3& nv);
    static Vector_3 normal_list27[27];
    static Vector_3 normal_list18[18];
    static Vector_3 normal_list11[11];
    static Vector_3 normal_list6[6];

    static bool isAngleLowerThan(Vector_3& nv1, Vector_3& nv2, double);
    static bool isAngleLowerThan(Vertex* origin, Vertex* v1, Vertex* v2, double);

    static double getSquaredDistance(Vertex* v1, Vertex* v2);
    CGALCalculation(){

    }
protected:

private:
};

#endif // VECTORCALCULATION_H
