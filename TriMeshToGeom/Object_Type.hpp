//
//  Object_Type.hpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 20..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef Object_Type_hpp
#define Object_Type_hpp

#include <vector>
#include <cstdlib>

#include "check.hpp"
#include "CombinedPolygon.hpp"
#include "Model.h"

// The object type
class obj_type{
public:
    OBJCollection* objcl;

    vector<polygon_type> tri_list;

    obj_type(){}

    void setCollection(OBJCollection* pob){
        objcl = pob;
    }

    //TODO - REMOVE
    char name[20];

    int vertices_qty;
    int polygons_qty;

    vertex_type* vertex;
    polygon_type* polygon;

    std::vector<CombinedPolygon*> polygon_list;
    Checker* checker;



    void setChecker(Checker* ck){
        checker = ck;
    }

    void print();
    void makePolygons();
    CombinedPolygon* makeOneBigPolygon(CombinedPolygon* thispl, bool* checked);
};


#endif /* Object_Type_hpp */
