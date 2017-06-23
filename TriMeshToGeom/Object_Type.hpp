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

    std::vector<polygon_type> tri_list;
    std::vector<vertex_type> *vertex_list_ptr;

    obj_type(){}

    Checker* checker;

    void setChecker(Checker* ck){
        checker = ck;
    }

    void setCollection(OBJCollection* pob){
        objcl = pob;
    }

    void setVertexList(std::vector<vertex_type> *vt_list){
        vertex_list_ptr = vt_list;
    }

    vertex_type* getVertex(unsigned long index);

    void print();
    std::vector<CombinedPolygon*> makePolygons();
    CombinedPolygon* makeOneBigPolygon(CombinedPolygon* thispl, bool* checked);
    bool checkDuplicateVertex();


};


#endif /* Object_Type_hpp */
