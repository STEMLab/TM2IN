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
    vector<vertex_type> vertex;
    vertex_type* getVertex(long index);
    vector<pair<string, vector<CombinedPolygon*>> > makeSurfaces(Checker check);
    int loadFile(char*);

private:
    void makeVertex(string&, vertex_type& );
    void makePolygon(string&, polygon_type& );
    string getGroupName(string&);
};

#endif /* TVRCollection_hpp */
