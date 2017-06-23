//
//  check.hpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 9..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef check_hpp
#define check_hpp

#include <stdio.h>
#include <vector>

#include "Model.h"
#include "Printer.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Kernel/global_functions.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;

class Checker{
private:
    double threshold_vertex = 0.0;
    double threshold_coplanar_degree = 0.0;

    static bool compare_vertex(vertex_type a, vertex_type b);

public:
    Checker(double a, double b){
        threshold_vertex = a;
        threshold_coplanar_degree = b;
    }

    bool isExistDuplication(std::vector<vertex_type> vertex);

    bool isSameX(vertex_type a, vertex_type b, vertex_type c);
    bool isSameY(vertex_type a, vertex_type b, vertex_type c);
    bool isSameZ(vertex_type a, vertex_type b, vertex_type c);

    bool isStraightXY(vertex_type a, vertex_type b, vertex_type c);
    bool isStraightXZ(vertex_type a, vertex_type b, vertex_type c);
    bool isStraightYZ(vertex_type a, vertex_type b, vertex_type c);

    bool isSameXY(vertex_type a, vertex_type b);
    bool isSameYZ(vertex_type a, vertex_type b);
    bool isSameXZ(vertex_type a, vertex_type b);

    bool isSameVertex(vertex_type& v1, vertex_type& v2);
    bool isSameVertex(vertex_type* v1, vertex_type* v2);
    bool isCoplanar(Vector_3& nv1, Vector_3& nv2);
};
#endif /* check_hpp */
