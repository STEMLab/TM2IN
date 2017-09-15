//
//  Surface.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "logic/check.hpp"
#include "data/Surface.hpp"
#include "logic/CleanPolygonMaker.h"
#include <cstdlib>

using namespace std;

Surface::Surface(Surface* cp){
    this->v_list = cp->v_list;
    this->av_normal = cp->av_normal;
    for (int i = 0 ; i < 3 ; i++){
        this->max_coords[i] = cp->max_coords[i];
        this->min_coords[i] = cp->min_coords[i];
    }
    this->sq_area = cp->sq_area;
}

Surface::Surface(Triangle* pl){
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

void Surface::setMBB(){
    for (int i = 0 ; i < 3 ; i++)
    {
        this->max_coords[i] = -1000000000;
        this->min_coords[i] = 1000000000;
    }

    for (unsigned int i = 0 ; i < this->v_list.size() ; i++){
        for (int j = 0 ; j < 3 ; j++){
            this->max_coords[j] = max(this->max_coords[j], this->v_list[i]->coords[j]);
            this->min_coords[j] = min(this->min_coords[j], this->v_list[i]->coords[j]);
        }
    }
}


void Surface::setMBB(Triangle* pl){
    Vertex* v[3] = {pl->a, pl->b, pl->c};
    for (int i = 0 ; i < 3 ; i++){
        for (int j = 0 ; j < 3 ; j++){
            this->max_coords[j] = max(this->max_coords[j], v[i]->coords[j]);
            this->min_coords[j] = min(this->min_coords[j], v[i]->coords[j]);
        }
    }
}

void Surface::setMBB(Surface* cp){
    for (int j = 0 ; j < 3 ; j++){
        this->max_coords[j] = max(this->max_coords[j], cp->max_coords[j]);
        this->min_coords[j] = min(this->min_coords[j], cp->min_coords[j]);
    }

}

void Surface::translate(double diff[]){
    for (int j = 0 ; j < 3 ; j++){
        this->max_coords[j] += diff[j];
        this->min_coords[j] += diff[j];
    }
}
/**
 * if this Vertex(add_id) try to make hole, return true.
 */
bool Surface::isExistSameVertexInRange(ll si, ll ei, Vertex* add_id){
    for (ll i = si ; i != ei ; i++)
    {
        if (i == (int)this->v_list.size()){
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


int Surface::getSegmentsNumber(ll si, ll ei){
    int num = 0;
    for (ll i = si ; ;){
        if (i == ei) break;
        if (num > (int)this->v_list.size()) {
            cout << "getSegmentsNumber Error" << endl;
            return -1;
        }
        num++;
        i++;
        if (i == (int)this->v_list.size()) i = 0;
    }
    return num;
}

bool Surface::checkDuplicate(Checker* ch){
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

void Surface::removeDuplication(Checker* ch){
    ull v_size = v_list.size();
    for (ull i = 0 ; i < v_size - 1; i++){
        if (ch->isSameVertex(v_list[i] , v_list[i+1])){
            v_list.erase(v_list.begin() + i + 1);
            i--;
            v_size -= 1;
        }
    }
}

void Surface::makeCoplanarParallelWithZ(){
    Point_3 center = getCenterPoint();
    Plane_3 plane;
    if (this->av_normal.z() > 0)
        plane = Plane_3(center, Vector_3(0,0,1));
    else if (this->av_normal.z() < 0){
        plane = Plane_3(center, Vector_3(0,0,-1));
    }
    else{
        cout << "Wrong in makeCoplanarParallelWithZ" << endl;
        exit(-1);
    }
    for (ull index = 0 ; index < this->v_list.size() ; index++ ){
        Point_3 point(this->v_list[index]->x(),this->v_list[index]->y(),this->v_list[index]->z());
        Point_3 projected = plane.projection(point);

        this->v_list[index]->setX(projected.x());
        this->v_list[index]->setY(projected.y());
        this->v_list[index]->setZ(projected.z());
//        Vertex* vt = new Vertex(projected.x(), projected.y(), projected.z());
//        this->v_list[index] = vt;
    }
}


Point_3 Surface::getCenterPoint(){
    double x=0.0,y=0.0,z=0.0;
    x = this->min_coords[0] + this->max_coords[0];
    y = this->min_coords[1] + this->max_coords[1];
    z = this->min_coords[2] + this->max_coords[2];
    x = x/2;
    y = y/2;
    z = z/2;
    Point_3 p(x,y,z);
    return p;
}


string Surface::toJSONString(){
    string ret;
    ret.append("{ \n");
    ret.append(" \"id\" : " + to_string(sf_id) );
    ret.append(" \n, \"normal\" : [");
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

bool Surface::updateNormal(Checker* ch){
    Vector_3 normal = Vector_3(0,0,0);
    for (int i = 1 ; i < (int)v_list.size() - 1; i++){
        Vector_3 new_normal = CGALCalculation::getCrossProduct(this->v_list[0], this->v_list[i], this->v_list[i+1]);
        normal = normal + new_normal;

    }

    if (normal == CGAL::NULL_VECTOR){
        return false;
    }
    else{
        this->av_normal = normal * (1 / sqrt(normal.squared_length()) );
        return true;
    }

}

bool Surface::isInMBB(Vertex* vt){
    if (vt->x() >= this->min_coords[0] && vt->x() <= this->max_coords[0]){
        if (vt->y() >= this->min_coords[1] && vt->y() <= this->max_coords[1]){
            if (vt->z() >= this->min_coords[2] && vt->z() <= this->max_coords[2]){
                return true;
            }
        }
    }
    return false;
}

bool Surface::compareLength(Surface* i, Surface* j) {
     return (i->getLength() > j->getLength());
}

bool Surface::compareArea(Surface* i, Surface* j) {
     return (i->sq_area > j->sq_area);
}

/**
*  Check that Surface is not valid. Straight Line or Point.
*/
bool Surface::isValid(){
    if (this->v_list.size() < 3) return false;

    bool isNOTcollinear = false;
    Point_3 start_p = CGALCalculation::makePoint(this->v_list[0]);
    Point_3 end_p = CGALCalculation::makePoint(this->v_list[1]);
    for (ll i = 1 ; i < (ll)this->v_list.size() - 1; i++){
        Point_3 mid_p(end_p.x(), end_p.y(), end_p.z());
        end_p = CGALCalculation::makePoint(this->v_list[i+1]);
        if (CGAL::collinear(start_p, mid_p, end_p)){
            continue;
        }
        else{
            isNOTcollinear = true;
        }
    }
    return isNOTcollinear;
}
