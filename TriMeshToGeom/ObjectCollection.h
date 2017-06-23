//
//  ObjectCollection.h
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef ObjectCollection_h
#define ObjectCollection_h

#include "Model.h"
#include "CombinedPolygon.hpp"

#include <vector>
#include <cstring>
#include <string>

using namespace std;

class OBJCollection{
public:
    vector<pair<string, obj_type*> > obj_list;
    virtual vector<pair<string, vector<CombinedPolygon*>> > makeSurfaces(Checker* check) = 0;
    virtual int loadFile(char* filename) = 0;
    virtual void print() = 0;
};

#endif /* ObjectCollection_h */
