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
    int makeSurfaces(Checker* check);
    void free();
    int cleaning(Checker* ch, int max_gener);
    int validate(Checker* ch);
    void makeGraph(Checker* ch);

    void test(int);
};

#endif /* ObjectCollection_h */
