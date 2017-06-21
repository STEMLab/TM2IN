//
//  printer.h
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 9..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef printer_h
#define printer_h

#include "Model.h"
#include <iostream>

using namespace std;

void printVertex(vertex_type* vt){
    cout << vt->x <<", "<< vt->y << ", " << vt->z << endl;
}

void printPolygonCoord(vertex_type* vt_arr, polygon_type* pl_arr){
    cout << pl_arr -> a << " : " ;
    printVertex(&vt_arr[pl_arr->a]);
    cout << pl_arr -> b << " : " ;
    printVertex(&vt_arr[pl_arr->b]);
    cout << pl_arr -> c << " : " ;
    printVertex(&vt_arr[pl_arr->c]);
}


#endif /* printer_h */
