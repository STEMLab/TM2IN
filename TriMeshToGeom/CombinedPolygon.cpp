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

    double max_x = max(max(va->x, vb->x), vc->x);
    double max_y = max(max(va->y, vb->y), vc->y);
    double max_z = max(max(va->z, vb->z), vc->z);
    double min_x = min(min(va->x, vb->x), vc->x);
    double min_y = min(min(va->y, vb->y), vc->y);
    double min_z = min(min(va->z, vb->z), vc->z);

    this->maxx = max_x;
    this->maxy = max_y;
    this->maxz = max_z;
    this->minx = min_x;
    this->miny = min_y;
    this->minz = min_z;

    Point_3 p3a(va->x,va->y,va->z );
    Point_3 p3b(vb->x,vb->y,vb->z );
    Point_3 p3c(vc->x,vc->y,vc->z );
    if (CGAL::collinear(p3a, p3b, p3c)){
        av_normal = CGAL::NULL_VECTOR;
    }
    else av_normal = CGAL::unit_normal(p3a,p3b,p3c);

}

void CombinedPolygon::setMBB(Triangle* pl){
    Vertex* va = pl->a;
    Vertex* vb = pl->b;
    Vertex* vc = pl->c;

    double max_x = max(max(va->x, vb->x), vc->x);
    double max_y = max(max(va->y, vb->y), vc->y);
    double max_z = max(max(va->z, vb->z), vc->z);
    double min_x = min(min(va->x, vb->x), vc->x);
    double min_y = min(min(va->y, vb->y), vc->y);
    double min_z = min(min(va->z, vb->z), vc->z);

    this->maxx = max(max_x, this->maxx);
    this->maxy = max(max_y, this->maxy);
    this->maxz = max(max_z, this->maxz);
    this->minx = min(min_x, this->minx);
    this->miny = min(min_y, this->miny);
    this->minz = min(min_z, this->minz);
}

bool CombinedPolygon::attachTriangle(Triangle* pl, Checker* ch)
{
    // check Trinagle is in near polygon or not
    if (!isNeighbor(pl)) return false;

    // TODO : overlap check (projection)
    Vector_3 pl_nv = pl->getNormal();
    if (pl_nv == CGAL::NULL_VECTOR)
    {
        cout << "NULL_VECTOR" << endl;
        return true;//Not combine but true
    }

    Vertex* add;
    ll index = findShareLine(pl, ch, &add);
    if (index == -1) return false;

    int share_two_line = isShareTwoLine(index, add); //before : 0, next : 1, no : -1
    if (share_two_line != -1)
    {
        setMBB(pl);
//        if (isShareThreeLine(index))
//        {
//            this->v_list.erase(v_list.begin() + index);
//            this->v_list.erase(v_list.begin() + index);
//            this->v_list.erase(v_list.begin() + index);
//            av_normal = av_normal + pl_nv;
//            return true;
//        }
        index += share_two_line;
        if (index >= (ll)this->v_list.size())
        {
            index -= (ll)this->v_list.size();
        }


        this->v_list.erase(v_list.begin() + index);
        av_normal = av_normal + pl_nv;

        return true;
    }


    if (checkMakeHole(index, add)) return false;
//    if (ch->isSameOrientation(pl_nv, this->av_normal))
//    {
    setMBB(pl);
    av_normal = av_normal + pl_nv;
    this->v_list.insert(v_list.begin() + index + 1, add);
    return true;
//    }


//    return false;
}

bool CombinedPolygon::isShareThreeLine(ll index){
    ll n_index = index + 3;
    if (n_index >= (int)this->v_list.size()) n_index -= this->v_list.size();

    return (this->v_list[index] == this->v_list[n_index]);

}

ll CombinedPolygon::findShareLine(Triangle* pl, Checker* ch, Vertex** add)
{
    for (ull id = 0 ; id < this->v_list.size() ; id++)
    {
        ull n_id = id + 1;
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

int CombinedPolygon::isShareTwoLine(ll index, Vertex* add_id){
    ll b_index = index - 1;
    if (b_index < 0) b_index = this->v_list.size() - 1;

    index += 2;
    if (index >= (ll)this->v_list.size()){
        index -= this->v_list.size();
    }

    if ( v_list[b_index] == add_id ) return 0;
    else if ( v_list[index] == add_id ) return 1;
    else return -1;
}

/**
 * if this Vertex(add_id) try to make hole, return false.
 */

bool CombinedPolygon::checkMakeHole(ll index, Vertex* add_id){
    for (int i = 0 ; i < this->v_list.size() ; i++)
    {
        if (v_list[i] == add_id)
        {
            index += 2;
            if (index >= (ll)this->v_list.size())
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

bool CombinedPolygon::checkDuplicate(Checker* ch){
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
    for (ull index = 0 ; index < this->v_list.size() ; index++ ){
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
    for (ull index = 0 ; index < this->v_list.size() ; index++ ){
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


bool CombinedPolygon::isNeighbor(Triangle* pl){
    Vertex* v_list[3] = {pl->a, pl->b, pl->c};

    for (int i = 0 ; i < 3 ; i++){
        if (isInMBB(v_list[i])){
            return true;
        }
    }

    return false;
}

bool CombinedPolygon::isInMBB(Vertex* vt){
    if (vt->x >= this->minx && vt->x <= this->maxx){
        if (vt->y >= this->miny && vt->y <= this->maxy){
            if (vt->z >= this->minz && vt->z <= this->maxz){
                return true;
            }
        }
    }
    return false;
}
