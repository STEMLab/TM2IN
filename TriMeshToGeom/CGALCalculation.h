#ifndef VECTORCALCULATION_H
#define VECTORCALCULATION_H

#include "vertex.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Kernel/global_functions.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Triangle_3 Triangle_3;

class CGALCalculation
{
public:
    static Vector_3 getNormal(Vertex* va, Vertex* vb, Vertex* vc);
    static double getAngle(Vector_3& v1, Vector_3& v2);

    static double getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3);
    static double getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc);

    static int findNormalType(Vector_3& nv);
    static Vector_3 normal_list[6];
protected:

private:
};

#endif // VECTORCALCULATION_H
