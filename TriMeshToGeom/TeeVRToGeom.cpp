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
#include "JSONMaker.h"
#include "TVRCollection.hpp"
#include "Object_Type.hpp"
#include "3dsloader.h"
#include "check.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    char path[] = "../Resource/teevr/tvr/test_tvr.tvr";
    //char path[] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/main_tvr.tvr";

    Checker* ch = new Checker(0.000001, 5.0);
    OBJCollection* objc = new TVRCollection();

    cout << "Load TVR File.." << endl;
    if (objc->loadFile(path, ch) == -1){
        cout << "Load File Error";
        return -1;
    }

    vector<pair<string, vector<CombinedPolygon*>> > cp = objc->makeSurfaces(ch);
    string json_file = "../polygon_temp.json";
    if (JSONMaker::printJSON(cp, json_file)) {
        return -1;
    }

    //objc->print();

    std::cout << "End!\n";
    return 0;
}
