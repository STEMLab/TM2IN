//
//  Object_Type.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 20..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "Object_Type.hpp"
#include "CombinedPolygon.hpp"

void obj_type::print(){
    
}

void obj_type::makePolygons(){
    std::vector<CombinedPolygon*> cpl_list;
    bool* checked = (bool*)malloc(sizeof(bool) * this->polygons_qty);
    
    for (int index = 0 ; index < this->polygons_qty; index++){
        if (checked[index]) continue;
        checked[index] = true ;
        cpl_list.push_back(
            makeOneBigPolygon(new CombinedPolygon(this->polygon[index], this), checked));
    }
    this->polygon_list = cpl_list;
}

CombinedPolygon* obj_type::makeOneBigPolygon(CombinedPolygon* cp, bool* checked){
    int count = 0;
    unsigned long pre_size = cp->getLength();
    
    for (int i = 0 ; i < this->polygons_qty ; i++){
        if (!checked[i]){
            if (cp->combine(this->polygon[i], this->checker)){
                
            }
        }
    }
    
    return cp;
}
