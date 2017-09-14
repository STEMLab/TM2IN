//
//  TVRCollection.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "data/ObjectCollection.h"


#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define FLOATSIZE 1000000

using namespace std;

int OBJCollection::makeSurfaces(Checker* check){

    for (auto it = this->space_list.begin() ; it != this->space_list.end() ; it++)
    {
        cout << (*it) -> name << " is converting..." << endl;
        int ret = (*it)->makeSurfacesGreedy();
        if (ret)
        {
            cout << "make Surfaces error" << endl;
            return -1;
        }

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
}

int OBJCollection::cleaning(Checker* ch){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        for (unsigned int i = 0 ; i < this->space_list[it]->polygon_list.size() ;i++){
            if (this->space_list[it]->polygon_list[i]->checkDuplicate(ch)){
                cout << "it has duplicate Vertex" << endl;
                return -1;
            }
        }
        if (this->space_list[it]->combineSurface() == -1)
        {
            cout << "combine error" << endl;
            return -1;
        }
        if (this->space_list[it]->simplifySegment() == -1)
        {
            cout << "simplify error" << endl;
            return -1;
        }
        if (this->space_list[it]->handleDefect() == -1){
            cout << "" << endl;
            return -1;
        }
        if (this->space_list[it]->match00() == -1)
        {
            cout << "match00 error" << endl;
            return -1;
        }


    }
    return 0;
}


