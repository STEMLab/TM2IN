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


using namespace std;

bool Checker::compare_vertex(Vertex* a, Vertex* b){
    if (a->x < b->x){
        return true;
    }
    else if (a->x > b->x){
        return false;
    }
    else{
        if (a->y < b->y)
        {
            return true;
        }
        else if (a->y > b->y)
        {
            return false;
        }
        else{
            if (a->z < b->z)
            {
                return true;
            }
            else
                return false;

        }
    }
}

bool Checker::isExistDuplication(vector<Vertex*> vertex){
//    sort(vertex.begin(), vertex.end(), this->compare_vertex);
//    for (int i = 0 ; i < vertex.size() - 1 ; i++)
//    {
//        int j = i + 1;
//        if (isSameVertex(vertex[i],vertex[j]))
//        {
//            cout << i << ", " << j << endl;
//            Printer::printVertex(vertex[i]);
//            Printer::printVertex(vertex[j]);
//            return true;
//        }
//    }
    return false;
}

bool Checker::isSameVertex(Vertex* v1, Vertex* v2){
    double thres = this->threshold_vertex;
    if (fabs(v1->z - v2->z) <= thres && fabs(v1->x - v2->x) <= thres && fabs(v1->y - v2->y) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameVertex(Vertex& v1, Vertex& v2){
    double thres = this->threshold_vertex;
    if (fabs(v1.z - v2.z) <= thres && fabs(v1.x - v2.x) <= thres && fabs(v1.y - v2.y) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameOrientation(Vector_3& nv1, Vector_3& nv2){
    double thres = this->ori_degree;
    double angle = CGALCalculation::getAngle(nv1, nv2);
    return (angle <= thres) ;
}

bool Checker::isSamePlanar(Vector_3& big, Vector_3& small){
    double thres = this->ori_degree;
    double angle = CGALCalculation::getAngle(big, small);
    Vector_3 added = big + small;
    if (angle <= 45.0){
        return (CGALCalculation::getAngle(added, big) <= thres);
    }
    return false;
}

bool Checker::isSimilarOrientation(Vector_3& nv1, Vector_3& nv2){
    double thres = this->ori_degree;
    double angle = CGALCalculation::getAngle(nv1, nv2);
    return (angle <= thres * 2) ;
}









//TODO

bool Checker::isSameXY(Vertex a, Vertex b){
    double thres = this->threshold_vertex;
    if (fabs(b.y - a.y) <= thres && fabs(b.x - a.x) <= thres){
        return true;
    }
    return false;
}

bool Checker::isSameYZ(Vertex a, Vertex b){
    double thres = this->threshold_vertex;
    if (fabs(b.y - a.y) <= thres && fabs(b.z - a.z) <= thres){
        return true;
    }
    return false;
}

bool Checker::isSameXZ(Vertex a, Vertex b){
    double thres = this->threshold_vertex;
    if (fabs(b.x - a.x) <= thres && fabs(b.z - a.z) <= thres){
        return true;
    }
    return false;
}

bool Checker::isStraightXY(Vertex a, Vertex b, Vertex c){
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

bool Checker::isStraightXZ(Vertex a, Vertex b, Vertex c){
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

bool Checker::isStraightYZ(Vertex a, Vertex b, Vertex c){
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

bool Checker::isSameX(Vertex a, Vertex b, Vertex c){
    double thres = this->threshold_vertex;
    if (fabs(a.x - b.x) <= thres && fabs(b.x-c.x) <= thres && fabs(c.x - a.x) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameY(Vertex a, Vertex b, Vertex c){
    double thres = this->threshold_vertex;
    if (fabs(a.y - b.y) <= thres && fabs(b.y-c.y) <= thres && fabs(c.y - a.y) <=thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameZ(Vertex a, Vertex b, Vertex c){
    double thres = this->threshold_vertex;
    if (fabs(a.z - b.z) <= thres && fabs(b.z - c.z) <= thres && fabs(c.z - a.z) <=thres){
        return true;
    }
    else
        return false;
}
