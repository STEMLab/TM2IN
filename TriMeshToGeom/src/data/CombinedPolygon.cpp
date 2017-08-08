//
//  CombinedPolygon.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "logic/check.hpp"
#include "data/CombinedPolygon.hpp"
#include "logic/CleanPolygonMaker.h"
#include <cstdlib>

using namespace std;

CombinedPolygon::CombinedPolygon(CombinedPolygon* cp){
    this->v_list = cp->v_list;
    this->av_normal = cp->av_normal;
    for (int i = 0 ; i < 3 ; i++){
        this->max_coords[i] = cp->max_coords[i];
        this->min_coords[i] = cp->min_coords[i];
    }
    this->sq_area = cp->sq_area;
}

CombinedPolygon::CombinedPolygon(Triangle* pl){
    Vertex* v[3] = {pl->a, pl->b, pl->c};

    for (int i = 0 ; i < 3 ;i++){
        v_list.push_back(v[i]);
    }

    double max_list[3] = {v[0]->x(), v[0]->y(), v[0]->z()};
    double min_list[3] = {v[0]->x(), v[0]->y(), v[0]->z()};
    for (int i = 1 ; i < 3 ; i++){
        for (int j = 0 ; j < 3 ; j++){
            max_list[j] = max(max_list[j], v[i]->coords[j]);
            min_list[j] = min(min_list[j], v[i]->coords[j]);
        }
    }

    for (int i = 0 ; i < 3 ; i++){
        this->max_coords[i] = max_list[i];
        this->min_coords[i] = min_list[i];
    }

    av_normal = pl->getNormal();
    sq_area = pl->getArea();
}

void CombinedPolygon::setMBB(Triangle* pl){
    Vertex* v[3] = {pl->a, pl->b, pl->c};
    for (int i = 0 ; i < 3 ; i++){
        for (int j = 0 ; j < 3 ; j++){
            this->max_coords[j] = max(this->max_coords[j], v[i]->coords[j]);
            this->min_coords[j] = min(this->min_coords[j], v[i]->coords[j]);
        }
    }
}

void CombinedPolygon::setMBB(CombinedPolygon* cp){
    for (int j = 0 ; j < 3 ; j++){
        this->max_coords[j] = max(this->max_coords[j], cp->max_coords[j]);
        this->min_coords[j] = min(this->min_coords[j], cp->min_coords[j]);
    }

}

/**
 * if this Vertex(add_id) try to make hole, return true.
 */
bool CombinedPolygon::isExistSameVertexInRange(ll si, ll ei, Vertex* add_id){
    for (ll i = si ; i != ei ; i++)
    {
        if (i == this->v_list.size()){
            i = -1;
            continue;
        }
        if (v_list[i] == add_id)
        {
            return true;
        }
    }
    return false;
}


int CombinedPolygon::getSegmentsNumber(ll si, ll ei){
    int num = 0;
    for (ll i = si ; ;){
        if (i == ei) break;
        if (num > this->v_list.size()) {
            cout << "getSegmentsNumber Error" << endl;
            return -1;
        }
        num++;
        i++;
        if (i == this->v_list.size()) i = 0;
    }
    return num;
}

bool CombinedPolygon::checkDuplicate(Checker* ch){
    vector<Vertex*> sorted_v_list(this->v_list);

    sort(sorted_v_list.begin(), sorted_v_list.end(), Vertex::compare);
    for (ull i = 0 ; i < sorted_v_list.size() - 1; i++){
        if (sorted_v_list[i] == sorted_v_list[i+1])
            return true;
        if (ch->isSameVertex(sorted_v_list[i], sorted_v_list[i+1]))
            return true;
    }
    return false;
}

void CombinedPolygon::makeCoplanar(){
    Point_3 center = getCenterPoint();
    Plane_3 plane(center, this->av_normal);
    for (ull index = 0 ; index < this->v_list.size() ; index++ ){
        Point_3 point(this->v_list[index]->x(),this->v_list[index]->y(),this->v_list[index]->z());
        Point_3 projected = plane.projection(point);
        Vertex* vertex = new Vertex(projected.x(), projected.y(), projected.z());
        this->v_list[index] = vertex;
    }
}


Point_3 CombinedPolygon::getCenterPoint(){
    double x=0.0,y=0.0,z=0.0;
    for (ull index = 0 ; index < this->v_list.size() ; index++ ){
        x += this->v_list[index]->x();
        y += this->v_list[index]->y();
        z += this->v_list[index]->z();
    }
    x = x/this->v_list.size();
    y = y/this->v_list.size();
    z = z/this->v_list.size();
    Point_3 p(x,y,z);
    return p;
}


string CombinedPolygon::toJSONString(){
    string ret;

    ret.append("{ \"normal\" : [");
    ret.append(to_string(this->av_normal.x()) + ", ");
    ret.append(to_string(this->av_normal.y()) + ", ");
    ret.append(to_string(this->av_normal.z()));
    ret.append("], \n");
    ret.append(" \"coord\" : [");
    for (unsigned int i = 0 ; i < this->v_list.size() ; i++){
        ret.append(this->v_list[i]->toJSON());
        ret.append(",");
    }
    ret.append(this->v_list[0]->toJSON());
    ret.append("] }");
    return ret;
}



bool CombinedPolygon::isInMBB(Vertex* vt){
    if (vt->x() >= this->min_coords[0] && vt->x() <= this->max_coords[0]){
        if (vt->y() >= this->min_coords[1] && vt->y() <= this->max_coords[1]){
            if (vt->z() >= this->min_coords[2] && vt->z() <= this->max_coords[2]){
                return true;
            }
        }
    }
    return false;
}

bool CombinedPolygon::compareLength(CombinedPolygon* i, CombinedPolygon* j) {
     return (i->getLength() > j->getLength());
}
