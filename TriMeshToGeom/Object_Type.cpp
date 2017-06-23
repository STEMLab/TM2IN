//
//  Object_Type.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 20..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "Object_Type.hpp"
#include "Printer.h"
#include "TVRCollection.hpp"
#include "CombinedPolygon.hpp"

void obj_type::print(){
    for (auto it = tri_list.begin() ; it != tri_list.end() ; it++){
        cout << endl;
        Printer::printPolygonCoord(this->vertex_list_ptr, *it);
    }
}

bool obj_type::checkDuplicateVertex(){
    return checker->isExistDuplication(*this->vertex_list_ptr);
}

std::vector<CombinedPolygon*> obj_type::makePolygons(){
    std::vector<CombinedPolygon*> cpl_list;
    bool* checked = (bool*)malloc(sizeof(bool) * this->tri_list.size());
    cout << "number : " << tri_list.size() << endl;
    for (unsigned long index = 0 ; index < this->tri_list.size(); index++){
        if (index % 1000 == 0) cout << "process : "<< index << endl;
        if (checked[index]) continue;
        checked[index] = true ;
        cpl_list.push_back(
            makeOneBigPolygon(new CombinedPolygon(this->tri_list[index], this), checked));
    }

    return cpl_list;
}


CombinedPolygon* obj_type::makeOneBigPolygon(CombinedPolygon* cp, bool* checked){
    for (unsigned long i = 0 ; i < this->tri_list.size() ; i++){
        if (!checked[i]){
            if (cp->combine(this->tri_list[i], this->checker)){
                checked[i] = true;
                return makeOneBigPolygon(cp, checked);
            }
        }
    }

    return cp;
}

vertex_type* obj_type::getVertex(unsigned long index){
    return &((*this->vertex_list_ptr)[index]);
}



