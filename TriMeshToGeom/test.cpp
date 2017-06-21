//
//  main.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 6..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include "model.h"
//#include "Object_Type.hpp"
//#include "3dsloader.h"
//#include "printer.h"
//#include "check.hpp"
//
//using namespace std;
//
//int main(int argc, const char * argv[]) {
//    char path[] = "../jongro/jongro.3DS";
//
//
//    vector<pair<string,obj_type*>> ret = Load3DS(path);
//
//    for (auto it = ret.begin(); it != ret.end() ; ++it){
//        cout<< " name : " << it->first <<endl;
//        it->second->setChecker(new Checker(0.1, 0.0));
//        it->second->makePolygons();
//        break;
//    }
//
//    std::cout << "Hello, World!\n";
//    return 0;
//}
