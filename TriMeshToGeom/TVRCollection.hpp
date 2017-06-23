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
    vector<vertex_type*> vertex;
    vector<pair<string, vector<CombinedPolygon*>> > makeSurfaces(Checker* check);
    int loadFile(char*, Checker* check);
    void print();

private:
    vertex_type* findSameVertex(Checker* check, vertex_type& vt);
    void makeVertex(string&, vertex_type& );
    void makeTriangle(string&, polygon_type& );
    string getGroupName(string&);
};

#endif /* TVRCollection_hpp */
