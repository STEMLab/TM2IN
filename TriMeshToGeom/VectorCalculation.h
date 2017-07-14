#ifndef VECTORCALCULATION_H
#define VECTORCALCULATION_H


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Kernel/global_functions.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;

class VectorCalculation
{
public:
    static double getAngle(Vector_3& v1, Vector_3& v2);
    static int findNormalType(Vector_3& nv);
    static Vector_3 normal_list[6];
protected:

private:
};

#endif // VECTORCALCULATION_H
