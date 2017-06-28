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

#include <cstdlib>

using namespace std;

bool CombinedPolygon::combine(polygon_type& pl, Checker* ch)
{
    //TODO
    unsigned long add_id;
    long index = findShareLine(pl, ch, add_id);
    if (index == -1) return false;

    if (isCoplanar(pl, ch))
    {
        if (isShareTwoLine(index, add_id)){
            if (index + 1 >= this->v_list.size())
            {
                index -= this->v_list.size();
            }
            Vector_3 pl_nv = this->getNormalVector(pl);
            if (pl_nv == CGAL::NULL_VECTOR) return true;
            this->v_list.erase(v_list.begin() + index);
            av_normal = av_normal + pl_nv;
            return true;
        }
        vertex_type* vn = obj->getVertex(add_id);
        Vector_3 pl_nv = this->getNormalVector(pl);
        if (pl_nv == CGAL::NULL_VECTOR) return true; //Not combine but true
        av_normal = av_normal + pl_nv;
        this->v_list.insert(v_list.begin() + index, vn);
        return true;
    }

    return false;
}

bool CombinedPolygon::isCoplanar(polygon_type& pl, Checker* ch){
    Vector_3 pl_nv = this->getNormalVector(pl);
    return ch->isCoplanar(pl_nv, av_normal);
}

bool CombinedPolygon::isShareTwoLine(long index, unsigned long add_id){
    if (index + 2 >= this->v_list.size()){
        index -= this->v_list.size();
    }
    return ( v_list[index] == obj->getVertex(add_id) );
}

Vector_3 CombinedPolygon::getNormalVector(polygon_type& pl){
    vertex_type* va = obj->getVertex(pl.a);
    vertex_type* vb = obj->getVertex(pl.b);
    vertex_type* vc = obj->getVertex(pl.c);

    Point_3 p3a(va->x,va->y,va->z);
    Point_3 p3b(vb->x,vb->y,vb->z);
    Point_3 p3c(vc->x,vc->y,vc->z);

    if (CGAL::collinear(p3a, p3b, p3c)){
        return CGAL::NULL_VECTOR;
    }
    return CGAL::unit_normal(p3a,p3b,p3c);
}

CombinedPolygon::CombinedPolygon(polygon_type& pl, obj_type* p_obj){
    obj = p_obj;
    vertex_type* va = p_obj->getVertex(pl.a);
    vertex_type* vb = p_obj->getVertex(pl.b);
    vertex_type* vc = p_obj->getVertex(pl.c);

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

long CombinedPolygon::findShareLine(polygon_type& pl, Checker* ch, unsigned long& add_id)
{
    for (unsigned long id = 0 ; id < v_list.size(); id++)
    {
        unsigned long n_id = id + 1;
        if (n_id == v_list.size()) n_id = 0;

        if (v_list[id] == obj->getVertex(pl.a)){
            if (v_list[n_id] == obj->getVertex(pl.b)){
                add_id = pl.c;
                return id;
            }
            else if(v_list[n_id] == obj->getVertex(pl.c)){
                add_id = pl.b;
                return id;
            }
        }
        if (v_list[id] == obj->getVertex(pl.b)){
            if (v_list[n_id] == obj->getVertex(pl.a)){
                add_id = pl.c;
                return id;
            }
            else if(v_list[n_id] == obj->getVertex(pl.c)){
                add_id = pl.a;
                return id;
            }
        }
        if (v_list[id] == obj->getVertex(pl.c)){
                if (v_list[n_id] == obj->getVertex(pl.b)){
                add_id = pl.a;
                return id;
            }
            else if(v_list[n_id] == obj->getVertex(pl.a)){
                add_id = pl.b;
                return id;
            }
        }
    }
    return -1;
}

string CombinedPolygon::toString(){
    string ret;
    for (unsigned int i = 0 ; i < this->v_list.size() ; i++){
        ret.append("[");
        double x = this->v_list[i]->x;
        double y = this->v_list[i]->y;
        double z = this->v_list[i]->z;
        ret.append(to_string(x));
        ret.append(",");
        ret.append(to_string(y));
        ret.append(",");
        ret.append(to_string(z));
        ret.append("] , ");
    }
    return ret;
}
