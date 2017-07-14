//
//  ObjectCollection.h
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef ObjectCollection_h
#define ObjectCollection_h

#include "CombinedPolygon.hpp"
#include "check.hpp"
#include "TriangleSpace.h"

#include <vector>
#include <cstring>
#include <string>

using namespace std;

class OBJCollection{
public:
    vector<Vertex*> vertex;
    vector<TriangleSpace*> space_list;
    int makeSurfaces(Checker* check);
    void removeVertexList();
    void cleaning(Checker* ch);
};

#endif /* ObjectCollection_h */
