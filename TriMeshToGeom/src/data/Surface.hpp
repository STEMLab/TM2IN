//
//  Surface.hpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 20..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef Surface_h
#define Surface_h

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


class Surface{
public:
    std::vector<Vertex*> v_list;
    Vector_3 av_normal = CGAL::NULL_VECTOR;

    double min_coords[3];
    double max_coords[3];
    double sq_area = 0.0;

    Surface(){ }

    Surface(Triangle* pl);
    Surface(Surface* cp);

    ull getLength(){
        return v_list.size();
    }

    bool attachTriangle(Triangle* pl, Checker* ch);
    bool attachTriangle2(Triangle* pl, Checker* ch);
    std::string toJSONString();
    void makeCoplanar();

    bool checkDuplicate(Checker* ch);
    bool isInMBB(Vertex* vt);
    void setMBB(Triangle* pl);
    void setMBB(Surface* pl);
    bool isExistSameVertexInRange(ll si, ll ei, Vertex* add_id);
    int getSegmentsNumber(ll si, ll ei);

    //compare vector size
    static bool compareLength(Surface* i, Surface* j);


private:
    Point_3 getCenterPoint();
    ll findShareLine(Triangle* pl, Checker* ch, Vertex** add);
    bool isShareThreeLine(ll index);
    int isShareTwoLine(ll index, Vertex* add);
    bool checkMakeHole(ll index, Vertex* add);

    bool isNeighbor(Triangle* pl);
};



#endif /* Surface_h */
