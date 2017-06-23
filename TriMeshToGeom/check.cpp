//
//  check.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 9..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "check.hpp"
#include <algorithm>
#include <cmath>

#define PI 3.14159265

using namespace std;

bool Checker::compare_vertex(vertex_type a, vertex_type b){
    if (a.x < b.x){
        return true;
    }
    else if (a.x > b.x){
        return false;
    }
    else{
        if (a.y < b.y)
        {
            return true;
        }
        else if (a.y > b.y)
        {
            return false;
        }
        else{
            if (a.z < b.z)
            {
                return true;
            }
            else
                return false;

        }
    }
}

bool Checker::isExistDuplication(vector<vertex_type> vertex){
    sort(vertex.begin(), vertex.end(), this->compare_vertex);
    for (int i = 0 ; i < vertex.size() - 1 ; i++)
    {
        int j = i + 1;
        if (vertex[i].x == vertex[j].x){
            if (vertex[i].y == vertex[j].y){
                if (vertex[i].z == vertex[j].z){
                    cout << i << ", " << j << endl;
                    Printer::printVertex(vertex[i]);
                    Printer::printVertex(vertex[j]);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Checker::isSameVertex(vertex_type* v1, vertex_type* v2){
    double thres = this->threshold_vertex;
    if (fabs(v1->z - v2->z) <= thres && fabs(v1->x - v2->x) <= thres && fabs(v1->y - v1->y) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameVertex(vertex_type& v1, vertex_type& v2){
    double thres = this->threshold_vertex;
    if (fabs(v1.z - v2.z) <= thres && fabs(v1.x - v2.x) <= thres && fabs(v1.y - v1.y) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isCoplanar(Vector_3& nv1, Vector_3& nv2){
    double thres = this->threshold_coplanar_degree;
    double cos = nv1 * nv2;
    double angle = acos(cos) * 180.0/PI;
    return (angle <= thres) ;
}












bool Checker::isSameXY(vertex_type a, vertex_type b){
    double thres = this->threshold_vertex;
    if (fabs(b.y - a.y) <= thres && fabs(b.x - a.x) <= thres){
        return true;
    }
    return false;
}

bool Checker::isSameYZ(vertex_type a, vertex_type b){
    double thres = this->threshold_vertex;
    if (fabs(b.y - a.y) <= thres && fabs(b.z - a.z) <= thres){
        return true;
    }
    return false;
}

bool Checker::isSameXZ(vertex_type a, vertex_type b){
    double thres = this->threshold_vertex;
    if (fabs(b.x - a.x) <= thres && fabs(b.z - a.z) <= thres){
        return true;
    }
    return false;
}

bool Checker::isStraightXY(vertex_type a, vertex_type b, vertex_type c){
//    double thres = this->threshold_coplanar;
//
//    if (isSameXY(a,b, thres)) return true;
//    if (isSameXY(b,c, thres)) return true;
//    if (isSameXY(a,c, thres)) return true;
//
//    if (thres < 1) thres = thres * thres;
//
//    double slope1 = (b.y - a.y) / (b.x - a.x);
//    double slope2 = (b.y - c.y) / (b.x - c.x);
//    double slope3 = (c.y - a.y) / (c.x - a.x);
//
//    if (fabs(slope1 - slope2) <= thres && fabs(slope2 - slope3) <= thres && fabs(slope3 - slope1) <= thres){
//        return true;
//    }
    return false;
}

bool Checker::isStraightXZ(vertex_type a, vertex_type b, vertex_type c){
//    double thres = this->threshold_coplanar;
//
//    if (isSameXZ(a,b, thres)) return true;
//    if (isSameXZ(b,c, thres)) return true;
//    if (isSameXZ(a,c, thres)) return true;
//
//    if (thres < 1) thres = thres * thres;
//
//    double slope1 = (b.z - a.z) / (b.x - a.x);
//    double slope2 = (b.z - c.z) / (b.x - c.x);
//    double slope3 = (c.z - a.z) / (c.x - a.x);
//
//    if (fabs(slope1 - slope2) <= thres && fabs(slope2 - slope3) <= thres && fabs(slope3 - slope1) <= thres){
//        return true;
//    }
    return false;
}

bool Checker::isStraightYZ(vertex_type a, vertex_type b, vertex_type c){
//    double thres = this->threshold_coplanar;
//
//    if (isSameYZ(a,b, thres)) return true;
//    if (isSameYZ(b,c, thres)) return true;
//    if (isSameYZ(a,c, thres)) return true;
//
//    if (thres < 1) thres = thres * thres;
//
//    double slope1 = (b.y - a.y) / (b.z - a.z);
//    double slope2 = (b.y - c.y) / (b.z - c.z);
//    double slope3 = (c.y - a.y) / (c.z - a.z);
//
//    if (fabs(slope1 - slope2) <= thres && fabs(slope2 - slope3) <= thres && fabs(slope3 - slope1) <= thres){
//        return true;
//    }
    return false;
}

bool Checker::isSameX(vertex_type a, vertex_type b, vertex_type c){
    double thres = this->threshold_vertex;
    if (fabs(a.x - b.x) <= thres && fabs(b.x-c.x) <= thres && fabs(c.x - a.x) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameY(vertex_type a, vertex_type b, vertex_type c){
    double thres = this->threshold_vertex;
    if (fabs(a.y - b.y) <= thres && fabs(b.y-c.y) <= thres && fabs(c.y - a.y) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameZ(vertex_type a, vertex_type b, vertex_type c){
    double thres = this->threshold_vertex;
    if (fabs(a.z - b.z) <= thres && fabs(b.z - c.z) <= thres && fabs(c.z - a.z) <=thres){
        return true;
    }
    else
        return false;
}
