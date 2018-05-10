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

#include <CGAL/Vector_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Kernel/global_functions.h>

class Checker{
private:
    Checker(){}

public:
    static double squaredDistanceOfSamePoint2D;
    static double thresholdVertex;
    static double threshold_1;
    static double degreeOfStraight;

    static bool isSameDouble(double a, double b);
    static bool isSameX(Vertex* a, Vertex* b);
    static bool isSameY(Vertex* a, Vertex* b);
    static bool isSameZ(Vertex* a, Vertex* b);

    static bool isSameX(Vertex* a, Vertex* b, Vertex* c);
    static bool isSameY(Vertex* a, Vertex* b, Vertex* c);
    static bool isSameZ(Vertex* a, Vertex* b, Vertex* c);

    static bool isSameVertex(Vertex& v1, Vertex& v2);
    static bool isSameVertex(Vertex* v1, Vertex* v2);

    static bool isCoplanar(Vector_3 &big, Vector_3 &small, double degree = -1.0);
    static bool isCollinear(Vertex *start_p, Vertex *check_p, Vertex *end_p);
    static bool checkMerge(Vector_3 &big, Vector_3 &small, double degree = -1.0);

    static int compare_vertex(Vertex* v1, Vertex* v2);

    static int num_of_straight;
    static int num_of_invalid;
    static double threshold_2;
};

#endif /* check_hpp */
