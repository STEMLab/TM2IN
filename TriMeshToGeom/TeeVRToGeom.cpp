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
#include "JSONMaker.h"
#include "TVRCollection.hpp"
#include "3dsloader.h"
#include "check.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    //Printer::extractMINtvr("../Resource/teevr/tvr/main_tvr"); return 0;
    //char path[] = "../Resource/teevr/tvr/main_tvr_2.min.tvr";
    char path[] = "../Resource/teevr/tvr/test_tvr.tvr";

    Checker* ch = new Checker(0.000001, 5.0);
    OBJCollection* objc = new TVRCollection();

    cout << "Load TVR File.." << endl;
    if (objc->loadFile(path, ch) == -1){
        cout << "Load File Error";
        return -1;
    }

    vector<pair<string, vector<CombinedPolygon*>> > cp = objc->makeSurfaces(ch);

    cout << "Cleaning..." << endl;
    for (unsigned int i = 0 ; i < cp.size() ; i++){
        for (unsigned int j = 0 ; j < cp[i].second.size() ; j++){
            cp[i].second[j]->cleaning(ch);
        }
    }

    cout << "Removing..." << endl;
    objc->removeVertexList();

    string json_file = "../polygon_temp.json";
    ofstream fout;
    fout.open(json_file, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printJSON(fout, cp[0].second)) {
        return -1;
    }
    fout.close();

    //objc->print();

    std::cout << "End!\n";
    return 0;
}
