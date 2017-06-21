//
//  CombinedPolygon.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "check.hpp"
#include "Object_Type.hpp"
#include "CombinedPolygon.hpp"
#include <CGAL/Cartesian.h>

using namespace std;

typedef CGAL::Cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;

bool CombinedPolygon::combine(polygon_type pl, Checker* ch){
    unsigned long index = findShareLine(pl, ch);
    if (index == -1) return false;
    
    
    
    return false;
}

CombinedPolygon::CombinedPolygon(polygon_type pl, obj_type* p_obj){
    obj = p_obj;
    v_list.push_back(&p_obj->vertex[pl.a]);
    v_list.push_back(&p_obj->vertex[pl.b]);
    v_list.push_back(&p_obj->vertex[pl.c]);
}

unsigned long CombinedPolygon::findShareLine(polygon_type pl, Checker* ch){
    for (int id = 0 ; id < v_list.size(); id++){
        int n_id = id + 1;
        if (n_id == v_list.size()) n_id = 0;
        
        if (ch->isSameVertex(*v_list[id],obj->vertex[pl.a]) &&
            ch->isSameVertex(*v_list[n_id],obj->vertex[pl.b] ) ){
            return id;
        }
        else if (ch->isSameVertex(*v_list[id],obj->vertex[pl.b]) &&
                 ch->isSameVertex(*v_list[n_id],obj->vertex[pl.a] ) ){
            return id;
        }else if (ch->isSameVertex(*v_list[id],obj->vertex[pl.a]) &&
                  ch->isSameVertex(*v_list[n_id],obj->vertex[pl.c] ) ){
            return id;
        }else if (ch->isSameVertex(*v_list[id],obj->vertex[pl.c]) &&
                  ch->isSameVertex(*v_list[n_id],obj->vertex[pl.a] ) ){
            return id;
        }else if (ch->isSameVertex(*v_list[id],obj->vertex[pl.b]) &&
                  ch->isSameVertex(*v_list[n_id],obj->vertex[pl.c] ) ){
            return id;
        }else if (ch->isSameVertex(*v_list[id],obj->vertex[pl.c]) &&
                  ch->isSameVertex(*v_list[n_id],obj->vertex[pl.b] ) ){
            return id;
        }
    }
    return -1;
}
