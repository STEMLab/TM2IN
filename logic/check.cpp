//
//  check.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 9..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "logic/check.h"
#include <algorithm>
#include <cmath>


using namespace std;

double Checker::threshold_same_vt_distance;
double Checker::squaredDistanceOfSamePoint2D;
double Checker::threshold_collinear;

int Checker::num_of_invalid = 0;
int Checker::num_of_straight = 0;

bool Checker::isCollinear(Vertex *start_p, Vertex *check_p, Vertex *end_p){
    return (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, Checker::threshold_collinear)
    || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, Checker::threshold_collinear)
    || CGALCalculation::isAngleLowerThan(check_p, end_p, start_p, Checker::threshold_collinear));
}

bool Checker::isSameVertex(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_same_vt_distance;
    return (fabs(v1->z() - v2->z()) < thres && fabs(v1->x() - v2->x()) < thres && fabs(v1->y() - v2->y()) < thres);
}

bool Checker::isSameVertex(Vertex& v1, Vertex& v2){
    double thres = Checker::threshold_same_vt_distance;
    return (fabs(v1.z() - v2.z()) < thres && fabs(v1.x() - v2.x()) < thres && fabs(v1.y() - v2.y()) < thres);
}





int Checker::compare_vertex(Vertex* a, Vertex* b)
{
    double x = a->x() - b->x();
    if (fabs(x) > threshold_same_vt_distance)
    {
        return x > 0.0 ? 1 : -1;
    }
    else
    {
        double y = a->y() - b->y();
        if (fabs(y) > threshold_same_vt_distance)
        {
            return y > 0.0 ? 1 : -1;
        }
        else
        {
            double z = a->z() - b->z();
            if (fabs(z) > threshold_same_vt_distance)
            {
                return z > 0.0 ? 1 : -1;
            }
            else
                return 0;
        }
    }
}


bool Checker::isSameX(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_same_vt_distance;
    return fabs(v1->x() - v2->x()) < thres;
}

bool Checker::isSameY(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_same_vt_distance;
    return fabs(v1->y() - v2->y()) < thres;
}

bool Checker::isSameZ(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_same_vt_distance;
    return fabs(v1->z() - v2->z()) < thres;
}



