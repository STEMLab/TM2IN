#ifndef VECTORCALCULATION_H
#define VECTORCALCULATION_H

#include "detail/cgal_config.h"
#include "features/Geometry.h"

#include <iostream>

class CGALCalculation
{
public:
    static double getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3);
    static double getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc);

    static Vector_3 getCrossProduct(Vertex* va, Vertex* vb, Vertex* vc);
protected:

private:
};

#endif // VECTORCALCULATION_H
