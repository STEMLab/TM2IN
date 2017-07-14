#ifndef TRINAGLE_H
#define TRINAGLE_H

#include "vertex.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Kernel/global_functions.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;

class Triangle{
private:
    Vector_3 normal = CGAL::NULL_VECTOR;
public:
    Vertex* a;
    Vertex* b;
    Vertex* c;

    Triangle();

    Triangle(Vertex* pa, Vertex *pb, Vertex* pc);

    Vector_3 getNormal();
    std::string toJSON();
};


#endif // TRINAGLE_H
