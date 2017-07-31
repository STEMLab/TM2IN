//
//  TVRCollection.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "ObjectCollection.h"


#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define FLOATSIZE 1000000

using namespace std;

int OBJCollection::makeSurfaces(Checker* check){

    for (auto it = this->space_list.begin() ; it != this->space_list.end() ; it++){
        cout << (*it) -> name << " is converting..." << endl;
        int ret = (*it)->makePolygons(check);
        if (ret) { cout << "make Surfaces error" << endl; return -1;}
    }

    return 0;
}


void OBJCollection::free(){
    sort( vertex.begin(), vertex.end() );
    vertex.erase( unique( vertex.begin(), vertex.end() ), vertex.end() );

    for (int i = 0 ; i < (int)this->vertex.size() ; i++){
        if (this->vertex[i] != NULL){
            delete this->vertex[i];
            this->vertex[i] = NULL;
        }

    }
    vertex.clear();

    for (auto it = space_list.begin() ; it != space_list.end() ; it++)
    {

    }
}

int OBJCollection::cleaning(Checker* ch){
    for (auto it = space_list.begin() ; it != space_list.end() ; it++)
    {
        if ((*it)->makePolygonsCoplanar(ch) == -1) return -1;
    }
    return 0;
}


