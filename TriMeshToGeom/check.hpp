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
#include "Model.h"

class Checker{
private:
    double threshold_vertex = 0.0;
    double threshold_coplanar = 0.0;
    
public:
    Checker(double a, double b){
        threshold_vertex = a;
        threshold_coplanar = b;
    }
    bool isSameX(vertex_type a, vertex_type b, vertex_type c);
    bool isSameY(vertex_type a, vertex_type b, vertex_type c);
    bool isSameZ(vertex_type a, vertex_type b, vertex_type c);
    
    bool isStraightXY(vertex_type a, vertex_type b, vertex_type c);
    bool isStraightXZ(vertex_type a, vertex_type b, vertex_type c);
    bool isStraightYZ(vertex_type a, vertex_type b, vertex_type c);
    
    bool isSameXY(vertex_type a, vertex_type b);
    bool isSameYZ(vertex_type a, vertex_type b);
    bool isSameXZ(vertex_type a, vertex_type b);
    
    bool isSameVertex(vertex_type v1, vertex_type v2);

};
#endif /* check_hpp */
