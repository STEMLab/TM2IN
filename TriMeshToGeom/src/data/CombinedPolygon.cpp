//
//  CombinedPolygon.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "logic/check.hpp"
#include "data/CombinedPolygon.hpp"

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

bool CombinedPolygon::attachTriangle(Triangle* pl, Checker* ch)
{
    // check Trinagle is in near polygon or not
    if (!isNeighbor(pl)) return false;

    // TODO : overlap check (projection)
    Vector_3 pl_nv = pl->getNormal();
//    if (pl_nv == CGAL::NULL_VECTOR)
//    {
//        cout << "NULL_VECTOR" << endl;
//        return true;/**< Not combine but true  */
//    }

    Vertex* add;
    ll index = findShareLine(pl, ch, &add);
    if (index == -1) return false;

    int share_two_line = isShareTwoLine(index, add); //before : 0, next : 1, no : -1
    if (share_two_line != -1)
    {
        setMBB(pl);
        index += share_two_line;
        if (index >= (ll)this->v_list.size())
        {
            index -= (ll)this->v_list.size();
        }

        this->v_list.erase(v_list.begin() + index);
        av_normal = av_normal + pl_nv;
        sq_area += pl->getArea();
        return true;
    }

    if (checkMakeHole(index, add)) return false;

    if (ch->isSamePlanar(pl_nv, this->av_normal))
    {
        setMBB(pl);
        av_normal = av_normal + pl_nv;
        sq_area += pl->getArea();
        this->v_list.insert(v_list.begin() + index + 1, add);

        return true;
    }


    return false;
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
 * if this Vertex(add_id) try to make hole, return true.
 */

bool CombinedPolygon::checkMakeHole(ll index, Vertex* add_id){
    for (ll i = 0 ; i < this->v_list.size() ; i++)
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
    Point_3 p3a(origin->x(),origin->y(),origin->z());
    Point_3 p3b(v1->x(),v1->y(),v1->z());
    Point_3 p3c(v2->x(),v2->y(),v2->z());

    Vector_3 vec1(p3a,p3b);
    Vector_3 vec2(p3a,p3c);

    return ch->isSameOrientation(vec1, vec2);
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
