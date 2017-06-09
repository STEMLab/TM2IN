//
//  main.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 6..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "model.h"
#include "load3ds.h"


using namespace std;

char Load3DS (obj_type* p_object, char *p_filename);


int main(int argc, const char * argv[]) {
    // insert code here...

    obj_type* object = (obj_type*)malloc(sizeof(obj_type));
    Load3DS(object, "../jongro/jongro.3DS");

    std::cout << "Hello, World!\n";
    return 0;
}


