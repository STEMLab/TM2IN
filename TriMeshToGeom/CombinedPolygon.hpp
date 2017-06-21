//
//  CombinedPolygon.hpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 20..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef CombinedPolygon_h
#define CombinedPolygon_h

#include "Model.h"
#include <vector>


class CombinedPolygon{
public:
    std::vector<vertex_type*> v_list;
    obj_type* obj;
    
    CombinedPolygon(){
        
    }
    
    CombinedPolygon(polygon_type pl, obj_type* p_obj);
    
    unsigned long getLength(){
        return v_list.size();
    }
    
    unsigned long findShareLine(polygon_type pl, Checker* ch);
    bool combine(polygon_type pl, Checker* ch);
};



#endif /* CombinedPolygon_h */
