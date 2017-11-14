//
//  ObjectCollection.h
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef ObjectCollection_h
#define ObjectCollection_h

#include "data/Surface.hpp"
#include "logic/check.hpp"
#include "data/Space.h"
#include "fileio/JSONMaker.h"
#include "predefine.h"

#include <vector>
#include <cstring>
#include <string>

using namespace std;

class OBJCollection{
public:
    vector<Vertex*> vertex;
    vector<Space*> space_list;
    int makeSurfaces(Checker* check, double degree);
    void free();
    int combineSurfaces(Checker* ch, int max_gener, double startDegree);
    int makeGraph(Checker* ch);

    void extractGeneration(int);
    void test();
};

#endif /* ObjectCollection_h */
