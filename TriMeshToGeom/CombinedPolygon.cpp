//
//  CombinedPolygon.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "check.hpp"
#include "CombinedPolygon.hpp"

#include <cstdlib>

using namespace std;



CombinedPolygon::CombinedPolygon(Triangle* pl){
    Vertex* va = pl->a;
    Vertex* vb = pl->b;
    Vertex* vc = pl->c;

    v_list.push_back(va);
    v_list.push_back(vb);
    v_list.push_back(vc);

    Point_3 p3a(va->x,va->y,va->z );
    Point_3 p3b(vb->x,vb->y,vb->z );
    Point_3 p3c(vc->x,vc->y,vc->z );
    if (CGAL::collinear(p3a, p3b, p3c)){
        av_normal = CGAL::NULL_VECTOR;
    }
    else av_normal = CGAL::unit_normal(p3a,p3b,p3c);

}


bool CombinedPolygon::attachTriangle(Triangle* pl, Checker* ch)
{
    // TODO overlap check (projection)
    Vector_3 pl_nv = pl->getNormal();
    if (ch->isSameOrientation(pl_nv, this->av_normal))
    {
        Vertex* add;
        long index = findShareLine(pl, ch, &add);

        if (index == -1 || checkMakeHole(index, add)) return false;

        Vector_3 pl_nv = pl->getNormal();
        if (pl_nv == CGAL::NULL_VECTOR) {
            cout << "NULL_VECTOR" << endl;
            return true;//Not combine but true
        }

        if (isShareTwoLine(index, add))
        {
            if (isShareThreeLine(index)){
                //cout << "\nCover Hole       ";
                this->v_list.erase(v_list.begin() + index);
                this->v_list.erase(v_list.begin() + index);
                this->v_list.erase(v_list.begin() + index);
                av_normal = av_normal + pl_nv;
                return true;
            }
            index++;
            if (index >= (long)this->v_list.size())
            {
                index -= (long)this->v_list.size();
            }

            this->v_list.erase(v_list.begin() + index);
            av_normal = av_normal + pl_nv;
            return true;
        }

        av_normal = av_normal + pl_nv;
        this->v_list.insert(v_list.begin() + index + 1, add);
        return true;
    }

    return false;
}

bool CombinedPolygon::isShareThreeLine(long index){
    long n_index = index + 3;
    if (n_index >= (int)this->v_list.size()) n_index -= this->v_list.size();

    return (this->v_list[index] == this->v_list[n_index]);

}

long CombinedPolygon::findShareLine(Triangle* pl, Checker* ch, Vertex** add)
{
    for (unsigned long id = 0 ; id < this->v_list.size() ; id++)
    {
        unsigned long n_id = id + 1;
        if (n_id == this->v_list.size()) n_id = 0;

        if (v_list[id] == pl->b){
            if (v_list[n_id] == pl->a){
                *add = pl->c;
                return id;
            }
        }
        if (v_list[id] == pl->c){
            if(v_list[n_id] == pl->b){
                *add = pl->a;
                return id;
            }
        }
        if (v_list[id] == pl->a){
            if(v_list[n_id] == pl->c){
                *add = pl->b;
                return id;
            }
        }
    }
    return -1;
}

bool CombinedPolygon::isShareTwoLine(long index, Vertex* add_id){
    index += 2;
    if (index >= (long)this->v_list.size()){
        index -= this->v_list.size();
    }
    return ( v_list[index] == add_id );
}

bool CombinedPolygon::checkMakeHole(long index, Vertex* add_id){
    for (int i = 0 ; i < this->v_list.size() ; i++)
    {
        if (v_list[i] == add_id)
        {
            index += 2;
            if (index >= (long)this->v_list.size())
            {
                index -= this->v_list.size();
            }
            if (index != i){
                return true;
            }
        }
    }
    return false;
}

bool CombinedPolygon::validate(Checker* ch){
    for (int i = 0 ; i < this->v_list.size() ; i++){
        for (int j = 0 ; j < this->v_list.size() ; j++){
            if (i == j) continue;
            if (v_list[i] == v_list[j]) return false;
            if (ch->isSameVertex(v_list[i], v_list[j])){
                cout << "checker think this is same" << endl;
                return false;
            }
        }
    }
    return true;
}

void CombinedPolygon::makeCoplanar(){
    Point_3 center = getCenterPoint();
    Plane_3 plane(center, this->av_normal);

//    int type = VectorCalculation::findNormalType(this->av_normal);
//    Plane_3 plane(center, VectorCalculation::normal_list[type]);
//this->av_normal = VectorCalculation::normal_list[type];
    for (unsigned long index = 0 ; index < this->v_list.size() ; index++ ){
        Point_3 point(this->v_list[index]->x,this->v_list[index]->y,this->v_list[index]->z);
        Point_3 projected = plane.projection(point);
        Vertex* vertex = new Vertex(projected.x(), projected.y(), projected.z());
        this->v_list[index] = vertex;
    }
}

void CombinedPolygon::simplify_colinear(Checker* ch)
{
    vector<Vertex*>::iterator it = this->v_list.begin();
    for ( ; it != this->v_list.end(); )
    {
        auto it2 = it + 1;
        auto it3 = it + 2;
        if (it == this->v_list.end() - 2)
        {
            it3 = v_list.begin();
        }
        if (it == this->v_list.end() - 1)
        {
            it2 = v_list.begin();
            it3 = v_list.begin() + 1;
        }

        bool condition = isSameOrientation(*it, *it2, *it3, ch);
        if (condition)
        {
            this->v_list.erase(it2);
            cout << "erase" << endl;
        }
        else
        {
            ++it;
        }
    }
}

bool CombinedPolygon::isSameOrientation(Vertex* origin, Vertex* v1, Vertex* v2, Checker* ch){
    Point_3 p3a(origin->x,origin->y,origin->z);
    Point_3 p3b(v1->x,v1->y,v1->z);
    Point_3 p3c(v2->x,v2->y,v2->z);

    Vector_3 vec1(p3a,p3b);
    Vector_3 vec2(p3a,p3c);

    return ch->isSameOrientation(vec1, vec2);
}

Point_3 CombinedPolygon::getCenterPoint(){
    double x=0.0,y=0.0,z=0.0;
    for (unsigned long index = 0 ; index < this->v_list.size() ; index++ ){
        x += this->v_list[index]->x;
        y += this->v_list[index]->y;
        z += this->v_list[index]->z;
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










