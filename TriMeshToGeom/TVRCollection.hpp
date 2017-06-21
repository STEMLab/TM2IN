//
//  TVRCollection.hpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef TVRCollection_hpp
#define TVRCollection_hpp

#include "Object_Type.hpp"
#include "ObjectCollection.h"

class TVRCollection : public OBJCollection{
public:
    vertex_type* getVertex(long index);
    vector<pair< string, CombinedPolygon*> > makeSurfaces(Checker check);
    void loadFile(char*);
};

#endif /* TVRCollection_hpp */
