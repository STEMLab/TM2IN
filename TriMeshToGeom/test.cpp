//
//  main.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 6..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//
#define threshold 0.1

#include <iostream>
#include <fstream>
#include "model.h"
#include "3dsloader.h"
#include "printer.h"
#include "check.hpp"

using namespace std;

polygon_type* polygon_arr[3][20000];

int main(int argc, const char * argv[]) {
    char path[] = "../jongro/jongro.3DS";

    
    obj_collection* cl = Load3DS(path);
    for (int i = 0 ; i < cl->num ; i++){
        obj_type* object = cl->list[i];
        int xy=0,xz= 0, yz=0;
        int nontype = 0;
        for (int pl = 0 ; pl < object->polygons_qty ; pl++){
            int type = findType(object, &object->polygon[pl], 0.05);
            switch (type) {
                case 0:
                    polygon_arr[type][xy++] = &object->polygon[pl];
                    break;
                case 1:
                    polygon_arr[type][xz++] = &object->polygon[pl];
                    break;
                case 2:
                    polygon_arr[type][yz++] = &object->polygon[pl];
                    break;
                    
                default:
                    cout << "non type polygon " << pl << endl;
                    printPolygonCoord(object->vertex, &object->polygon[pl]);
                    nontype++;
                    break;
            }
        }
        
        cout << "xy : " << xy << endl;
        cout << "xz : " << xz << endl;
        cout << "yz : " << yz <<endl;
        cout << nontype << endl;
    }
    
/*
    int xy=0,xz= 0, yz=0;
    int nontype = 0;
    for (int pl = 0 ; pl < object->polygons_qty ; pl++){
        int type = findType(object, &object->polygon[pl], 0.05);
        switch (type) {
            case 0:
                polygon_arr[type][xy++] = &object->polygon[pl];
                break;
            case 1:
                polygon_arr[type][xz++] = &object->polygon[pl];
                break;
            case 2:
                polygon_arr[type][yz++] = &object->polygon[pl];
                break;

            default:
                cout << "non type polygon " << pl << endl;
                printPolygonCoord(object->vertex, &object->polygon[pl]);
                nontype++;
                break;
        }
    }
 
    cout << "xy : " << xy << endl;
    cout << "xz : " << xz << endl;
    cout << "yz : " << yz <<endl;
    cout << nontype << endl;
 */
    
    std::cout << "Hello, World!\n";
    return 0;
}
