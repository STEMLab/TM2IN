#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "features/vertex.h"
#include "predefine.h"
#include "logic/CGALCalculation.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Kernel/global_functions.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Vector_3 Vector_3;

class Triangle{
private:
    Vector_3 normal = CGAL::NULL_VECTOR;
    double area = 0.0;
public:
    std::vector<HalfEdge* > edges;

    Triangle(){}

    Triangle(Vertex* pa, Vertex *pb, Vertex* pc);

    bool checkAndSetAdjacent(Triangle *tri);
    bool isOpposite(Triangle* tri);
    int findShareSameHalfEdge(Triangle *pTriangle);

    Vertex* operator[](int idx);
    Vertex* vertex(int idx);

    Vector_3 getNormal();
    double getArea();
    std::string toJSON();
};


#endif // TRIANGLE_H
