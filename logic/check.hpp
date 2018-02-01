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

#include "features/Vertex.h"
#include "cgal/CGALCalculation.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Kernel/global_functions.h>

class Checker{
private:
    Checker(){}

public:
    static double squaredDistanceOfSamePoint2D;
    static double threshold_vertex;
    static double degreeOfMerging;
    static double degreeOfStraight;

//    Checker(double vertex){
//        this->threshold_vertex = vertex;
//        this->degreeOfMerging = 60.0;
//    }

    static bool isSameDouble(double a, double b);
    static bool isSameX(Vertex* a, Vertex* b);
    static bool isSameY(Vertex* a, Vertex* b);
    static bool isSameZ(Vertex* a, Vertex* b);

    static bool isSameX(Vertex* a, Vertex* b, Vertex* c);
    static bool isSameY(Vertex* a, Vertex* b, Vertex* c);
    static bool isSameZ(Vertex* a, Vertex* b, Vertex* c);

    static bool isSameVertex(Vertex& v1, Vertex& v2);
    static bool isSameVertex(Vertex* v1, Vertex* v2);

    static bool CanbeMerged(Vector_3& big, Vector_3& small, double degree);

    static bool isCollinear(Vertex* origin, Vertex* v1, Vertex* v2);

    static int compare_vertex(Vertex* v1, Vertex* v2);
};

#endif /* check_hpp */
