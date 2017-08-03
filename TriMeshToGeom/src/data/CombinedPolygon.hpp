//
//  CombinedPolygon.hpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 20..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef CombinedPolygon_h
#define CombinedPolygon_h

#include <vector>
#include <string>
#include <climits>

#include "model/Trinagle.h"
#include "logic/check.hpp"
#include "logic/util.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Kernel/global_functions.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Plane_3 Plane_3;


class CombinedPolygon{
public:
    std::vector<Vertex*> v_list;
    Vector_3 av_normal = CGAL::NULL_VECTOR;
    double minx=0, miny=0, minz=0;
    double maxx=0, maxy=0, maxz=0;
    double sq_area = 0.0;

    CombinedPolygon(){ }

    CombinedPolygon(Triangle* pl);
    CombinedPolygon(CombinedPolygon* cp);

    ull getLength(){
        return v_list.size();
    }
    bool attachTriangle(Triangle* pl, Checker* ch);
    bool attachPolygon(CombinedPolygon* cp, Checker* ch);
    std::string toJSONString();
    void makeCoplanar();
    void simplify_colinear(Checker* ch);

    bool checkDuplicate(Checker* ch);
private:
    Point_3 getCenterPoint();
    ll findShareLine(Triangle* pl, Checker* ch, Vertex** add);
    bool isSameOrientation(Vertex* origin, Vertex* v1, Vertex* v2, Checker* ch);
    bool isShareThreeLine(ll index);
    int isShareTwoLine(ll index, Vertex* add);
    bool checkMakeHole(ll index, Vertex* add);
    void setMBB(Triangle* pl);
    bool isNeighbor(Triangle* pl);
    bool isNeighbor(CombinedPolygon* cp);
    bool isInMBB(Vertex* vt);
};



#endif /* CombinedPolygon_h */
