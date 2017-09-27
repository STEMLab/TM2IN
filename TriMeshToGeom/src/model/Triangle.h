#ifndef TRINAGLE_H
#define TRINAGLE_H

#include "model/vertex.h"
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
    Vertex* a;
    Vertex* b;
    Vertex* c;

    Triangle();

    Triangle(Vertex* pa, Vertex *pb, Vertex* pc);

    Vector_3 getNormal();
    double getArea();
    std::string toJSON();
    bool isOpposite(Triangle& tri);
};


#endif // TRINAGLE_H
