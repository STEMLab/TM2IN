//
//  check.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 9..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "logic/check.hpp"
#include <algorithm>
#include <cmath>


using namespace std;

double Checker::threshold_vertex = 0.0;
double Checker::squaredDistanceOfSamePoint2D = 0.0;
double Checker::degreeOfMerging = 10.0;
double Checker::degreeOfStraight = 0.01;

bool Checker::isCollinear(Vertex *start_p, Vertex *check_p, Vertex *end_p){
    return (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, Checker::degreeOfStraight)
    || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, Checker::degreeOfStraight)
    || CGALCalculation::isAngleLowerThan(check_p, end_p, start_p, Checker::degreeOfStraight));
}

bool Checker::isSameDouble(double a, double b){
    return (fabs(a-b) < threshold_vertex);
}

bool Checker::isSameVertex(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_vertex;
    return (fabs(v1->z() - v2->z()) < thres && fabs(v1->x() - v2->x()) < thres && fabs(v1->y() - v2->y()) < thres);
}

bool Checker::isSameVertex(Vertex& v1, Vertex& v2){
    double thres = Checker::threshold_vertex;
    return (fabs(v1.z() - v2.z()) < thres && fabs(v1.x() - v2.x()) < thres && fabs(v1.y() - v2.y()) < thres);
}


bool Checker::CanbeMerged(Vector_3& big, Vector_3& small, double degree){
    double angle = CGALCalculation::getAngle(big, small);
    Vector_3 added = big + small;
    if (angle <= Checker::degreeOfMerging)
        return (CGALCalculation::getAngle(added, big) <= degree);
    return false;
}

int Checker::compare_vertex(Vertex* a, Vertex* b)
{
    double x = a->x() - b->x();
    if (fabs(x) > threshold_vertex)
    {
        return x > 0.0 ? 1 : -1;
    }
    else
    {
        double y = a->y() - b->y();
        if (fabs(y) > threshold_vertex)
        {
            return y > 0.0 ? 1 : -1;
        }
        else
        {
            double z = a->z() - b->z();
            if (fabs(z) > threshold_vertex)
            {
                return z > 0.0 ? 1 : -1;
            }
            else
                return 0;
        }
    }
}


bool Checker::isSameX(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_vertex;
    if (fabs(v1->x() - v2->x()) < thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameY(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_vertex;
    if (fabs(v1->y() - v2->y()) < thres){
        return true;
    }
    else
        return false;
}

bool Checker::isSameZ(Vertex* v1, Vertex* v2){
    double thres = Checker::threshold_vertex;
    if (fabs(v1->z() - v2->z()) < thres){
        return true;
    }
    else
        return false;
}



