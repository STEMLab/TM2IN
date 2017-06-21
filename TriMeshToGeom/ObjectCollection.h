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

#include <vector>


using namespace std;

class OBJCollection{
public:
    vector<pair<string, obj_type*> > obj_list;
    vector<vector<vertex_type>> vertex;
    
    virtual vertex_type* getVertex(long index) = 0;
    virtual vector<pair< string, CombinedPolygon*> > makeSurfaces(Checker check) = 0;
    virtual void loadFile(char* filename) = 0;
};

#endif /* ObjectCollection_h */
