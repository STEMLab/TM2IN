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
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;

class CombinedPolygon{
public:
    std::vector<vertex_type*> v_list;
    obj_type* obj;
    Vector_3 av_normal;

    CombinedPolygon(){

    }

    CombinedPolygon(polygon_type& pl, obj_type* p_obj);

    unsigned long getLength(){
        return v_list.size();
    }

    long findShareLine(polygon_type pl, Checker* ch, unsigned long& add_id);
    bool combine(polygon_type pl, Checker* ch);
    bool isCoplanar(polygon_type pl, Checker* ch);
    Vector_3 getNormalVector(polygon_type& pl);
};



#endif /* CombinedPolygon_h */
