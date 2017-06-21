//
//  TVRCollection.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "TVRCollection.hpp"

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

vertex_type* TVRCollection::getVertex(long index){
    //TODO
    return NULL;
}

vector<pair< string, CombinedPolygon*> > TVRCollection::makeSurfaces(Checker check){
    //TODO
    return {};
}

void TVRCollection::loadFile(char *filename){
    ifstream fin(filename);
    
    string inputstr;
    int count = 0;
    while(!fin.eof()){
        getline(fin, inputstr);
        cout << inputstr <<", " << count << endl;
        if (count == 1000) break;
        count ++;
    }
}
