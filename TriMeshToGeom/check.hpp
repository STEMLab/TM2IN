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
#include "model.h"

int findAxis(obj_type* obj, polygon_type* polygon, double thres = 0);
int findType(obj_type* obj, polygon_type* polygon, double thres = 0);

bool isSameX(vertex_type a, vertex_type b, vertex_type c, double thres = 0);
bool isSameY(vertex_type a, vertex_type b, vertex_type c, double thres = 0);
bool isSameZ(vertex_type a, vertex_type b, vertex_type c, double thres = 0);

bool isStraightXY(vertex_type a, vertex_type b, vertex_type c, double thres = 0);
bool isStraightXZ(vertex_type a, vertex_type b, vertex_type c, double thres = 0);
bool isStraightYZ(vertex_type a, vertex_type b, vertex_type c, double thres = 0);

bool isSameXY(vertex_type a, vertex_type b, double thres);
bool isSameYZ(vertex_type a, vertex_type b, double thres);
bool isSameXZ(vertex_type a, vertex_type b, double thres);

#endif /* check_hpp */
