//
//  TeeVRToGeom.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

#include "ObjectCollection.h"
#include "Model.h"
#include "TVRCollection.hpp"
#include "Object_Type.hpp"
#include "3dsloader.h"
#include "check.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    char path[] = "../Resource/teevr/tvr/main_tvr.tvr";
    //char path[] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/main_tvr.tvr";

    OBJCollection* objc = new TVRCollection();
    if (objc->loadFile(path) == -1){
        cout << "Load File Error";
        return -1;
    }

    vector<pair<string, vector<CombinedPolygon*>> > cp = objc->makeSurfaces(new Checker(0.0, 5.0));


    //objc->print();

    std::cout << "Hello, World!\n";
    return 0;
}
