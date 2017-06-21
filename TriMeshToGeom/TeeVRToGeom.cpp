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
#include "Printer.h"
#include "check.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    //char path[] = "Resource/teevr/tvr/main_tvr.tvr";
    char path[] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/main_tvr.tvr";
    
    OBJCollection* objc = new TVRCollection();
    objc->loadFile(path);
    
//    for (auto it = ret.begin(); it != ret.end() ; ++it){
//        cout<< " name : " << it->first <<endl;
//        it->second->setChecker(new Checker(0.1, 0.0));
//        it->second->makePolygons();
//        break;
//    }
    
    std::cout << "Hello, World!\n";
    return 0;
}
