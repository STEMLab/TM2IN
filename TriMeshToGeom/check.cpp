//
//  check.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 9..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "check.hpp"
#include <cmath>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

/*
 0 : XY
 1 : XZ
 2 : YZ
 3 : None
 */
int findType(obj_type* obj, polygon_type* poly, double thres){
    vertex_type a = obj->vertex[poly->a];
    vertex_type b = obj->vertex[poly->b];
    vertex_type c = obj->vertex[poly->c];
   
    if (isSameZ(a,b,c,thres)){ //WALL
        return 0;
    }
    else if (isSameY(a,b,c,thres) ){
        return 1;
    }
    else if (isSameX(a,b,c,thres)){
        return 2;
    }
    
    if (isStraightXY(a,b,c,thres)){ //WALL
        return 0;
    }
    else if (isStraightXZ(a, b, c, thres) ){
        return 1;
    }
    else if (isStraightYZ(a, b, c, thres)){
        return 2;
    }
    return 3;
    
}

bool isSameXY(vertex_type a, vertex_type b, double thres){
    if (fabs(b.y - a.y) <= thres && fabs(b.x - a.x) <= thres){
        return true;
    }
    return false;
}

bool isSameYZ(vertex_type a, vertex_type b, double thres){
    if (fabs(b.y - a.y) <= thres && fabs(b.z - a.z) <= thres){
        return true;
    }
    return false;
}

bool isSameXZ(vertex_type a, vertex_type b, double thres){
    if (fabs(b.x - a.x) <= thres && fabs(b.z - a.z) <= thres){
        return true;
    }
    return false;
}

bool isStraightXY(vertex_type a, vertex_type b, vertex_type c, double thres){
    if (isSameXY(a,b, thres)) return true;
    if (isSameXY(b,c, thres)) return true;
    if (isSameXY(a,c, thres)) return true;
    
    if (thres < 1) thres = thres * thres;
    
    double slope1 = (b.y - a.y) / (b.x - a.x);
    double slope2 = (b.y - c.y) / (b.x - c.x);
    double slope3 = (c.y - a.y) / (c.x - a.x);
    
    if (fabs(slope1 - slope2) <= thres && fabs(slope2 - slope3) <= thres && fabs(slope3 - slope1) <= thres){
        return true;
    }
    return false;
}

bool isStraightXZ(vertex_type a, vertex_type b, vertex_type c, double thres){
    if (isSameXZ(a,b, thres)) return true;
    if (isSameXZ(b,c, thres)) return true;
    if (isSameXZ(a,c, thres)) return true;
    
    if (thres < 1) thres = thres * thres;
    
    double slope1 = (b.z - a.z) / (b.x - a.x);
    double slope2 = (b.z - c.z) / (b.x - c.x);
    double slope3 = (c.z - a.z) / (c.x - a.x);
    
    if (fabs(slope1 - slope2) <= thres && fabs(slope2 - slope3) <= thres && fabs(slope3 - slope1) <= thres){
        return true;
    }
    return false;
}

bool isStraightYZ(vertex_type a, vertex_type b, vertex_type c, double thres){
    if (isSameYZ(a,b, thres)) return true;
    if (isSameYZ(b,c, thres)) return true;
    if (isSameYZ(a,c, thres)) return true;
    
    if (thres < 1) thres = thres * thres;
    
    double slope1 = (b.y - a.y) / (b.z - a.z);
    double slope2 = (b.y - c.y) / (b.z - c.z);
    double slope3 = (c.y - a.y) / (c.z - a.z);
    
    if (fabs(slope1 - slope2) <= thres && fabs(slope2 - slope3) <= thres && fabs(slope3 - slope1) <= thres){
        return true;
    }
    return false;
}

int findAxis(obj_type* obj, polygon_type* poly, double thres){
    vertex_type a = obj->vertex[poly->a];
    vertex_type b = obj->vertex[poly->b];
    vertex_type c = obj->vertex[poly->c];
    
    if (isSameX(a,b,c,thres)){
        return 0;
    }
    else if (isSameY(a,b,c,thres)){
        return 1;
    }
    else if (isSameZ(a,b,c,thres)){
        return 2;
    }
    else{
        return -1;
    }
}

bool isSameX(vertex_type a, vertex_type b, vertex_type c, double thres){
    if (fabs(a.x - b.x) <= thres && fabs(b.x-c.x) <= thres && fabs(c.x - a.x) <=thres){
        return true;
    }
    else
        return false;
}

bool isSameY(vertex_type a, vertex_type b, vertex_type c, double thres){
    if (fabs(a.y - b.y) <= thres && fabs(b.y-c.y) <= thres && fabs(c.y - a.y) <=thres){
        return true;
    }
    else
        return false;
}

bool isSameZ(vertex_type a, vertex_type b, vertex_type c, double thres){
    if (fabs(a.z - b.z) <= thres && fabs(b.z - c.z) <= thres && fabs(c.z - a.z) <=thres){
        return true;
    }
    else
        return false;
}
