#ifndef VECTORCALCULATION_H
#define VECTORCALCULATION_H

#include "model/vertex.h"
#include "data/Surface.hpp"

#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>
#include <CGAL/Point_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Kernel/global_functions.h>
#include <CGAL/squared_distance_3.h>

//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Delaunay_triangulation_3.h>
//#include <CGAL/Triangulation_vertex_base_with_info_3.h>
//
//typedef CGAL::Exact_predicates_inexact_constructions_kernel            T_Kernel;
//typedef CGAL::Triangulation_vertex_base_with_info_3<unsigned int, T_Kernel> Vb;
//typedef CGAL::Triangulation_data_structure_3<Vb>                       Tds;
//typedef CGAL::Delaunay_triangulation_3<T_Kernel, Tds>                    Delaunay;
//typedef T_Kernel::Point_3                                                T_Point;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Triangle_3 Triangle_3;
typedef Kernel::Line_3 Line_3;



class CGALCalculation
{
public:
    static std::vector<std::vector<int>> triangulate(Surface* sf);
//    static T_Point makeTPoint(Vertex* vt);
    static Vector_3 getUnitNormal(Vertex* va, Vertex* vb, Vertex* vc);
    static double getAngle(Vector_3& v1, Vector_3& v2);

    static double getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3);
    static double getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc);

    static Vector_3 getCrossProduct(Vertex* va, Vertex* vb, Vertex* vc);

    static int findNormalType27(Vector_3& nv);
    static int findNormalType10(Vector_3& nv);
    static Vector_3 normal_list27[27];
    static Vector_3 normal_list11[11];
    static Point_3 makePoint(Vertex* v);

    static double getSquaredDistance(Vertex* v1, Vertex* v2);
    CGALCalculation(){

    }
protected:

private:
};

#endif // VECTORCALCULATION_H
