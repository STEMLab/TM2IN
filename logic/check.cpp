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

double Checker::thresholdVertex;
double Checker::squaredDistanceOfSamePoint2D;
double Checker::coplanar_degree;
double Checker::degreeOfStraight;
double Checker::merge_degree;

int Checker::num_of_invalid = 0;
int Checker::num_of_straight = 0;

bool Checker::isCollinear(Vertex *start_p, Vertex *check_p, Vertex *end_p){
    return (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, Checker::degreeOfStraight)
    || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, Checker::degreeOfStraight)
    || CGALCalculation::isAngleLowerThan(check_p, end_p, start_p, Checker::degreeOfStraight));
}

bool Checker::isSameVertex(Vertex* v1, Vertex* v2){
    double thres = Checker::thresholdVertex;
    return (fabs(v1->z() - v2->z()) < thres && fabs(v1->x() - v2->x()) < thres && fabs(v1->y() - v2->y()) < thres);
}

bool Checker::isSameVertex(Vertex& v1, Vertex& v2){
    double thres = Checker::thresholdVertex;
    return (fabs(v1.z() - v2.z()) < thres && fabs(v1.x() - v2.x()) < thres && fabs(v1.y() - v2.y()) < thres);
}

bool Checker::isCoplanar(Vector_3 &big, Vector_3 &small, double degree) {
    double angle = CGALCalculation::getAngle(big, small);
    if (degree < 0) return angle <= Checker::coplanar_degree;
    else return angle <= degree;
}

bool Checker::checkMerge(Vector_3 &big, Vector_3 &small, double degree) {
    Vector_3 added = big + small;
    if (Checker::isCoplanar(big, small)){
        double angle = CGALCalculation::getAngle(added, big);
        if (degree < 0) return (angle <= Checker::merge_degree);
        else return angle <= degree;
    }
    return false;
}

int Checker::compare_vertex(Vertex* a, Vertex* b)
{
    double x = a->x() - b->x();
    if (fabs(x) > thresholdVertex)
    {
        return x > 0.0 ? 1 : -1;
    }
    else
    {
        double y = a->y() - b->y();
        if (fabs(y) > thresholdVertex)
        {
            return y > 0.0 ? 1 : -1;
        }
        else
        {
            double z = a->z() - b->z();
            if (fabs(z) > thresholdVertex)
            {
                return z > 0.0 ? 1 : -1;
            }
            else
                return 0;
        }
    }
}


bool Checker::isSameX(Vertex* v1, Vertex* v2){
    double thres = Checker::thresholdVertex;
    return fabs(v1->x() - v2->x()) < thres;
}

bool Checker::isSameY(Vertex* v1, Vertex* v2){
    double thres = Checker::thresholdVertex;
    return fabs(v1->y() - v2->y()) < thres;
}

bool Checker::isSameZ(Vertex* v1, Vertex* v2){
    double thres = Checker::thresholdVertex;
    return fabs(v1->z() - v2->z()) < thres;
}



