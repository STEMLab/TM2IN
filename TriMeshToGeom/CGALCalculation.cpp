#include "CGALCalculation.h"

#define PI 3.14159265

double CGALCalculation::getAngle(Vector_3& nv1, Vector_3& nv2){
    double cos = (nv1 * nv2) / ( sqrt(nv1.squared_length()) * sqrt(nv2.squared_length()) );
    return acos(cos) * 180.0/PI;
}

// 0 : up, down, left , right, front, back
int CGALCalculation::findNormalType(Vector_3& nv)
{
    int type = 0;
    double diff = 90.0;
    for (int i = 0 ; i < 6 ; i++){
        double temp_diff = CGALCalculation::getAngle(CGALCalculation::normal_list[i], nv);
        if (temp_diff < diff){
            diff = temp_diff;
            type = i;
        }
    }
    return type;
}

Vector_3 CGALCalculation::normal_list[6] = {
    Vector_3(0,0,1),
    Vector_3(0,0,-1),
    Vector_3(1,0,0),
    Vector_3(-1,0,0),
    Vector_3(0,1,0),
    Vector_3(0,-1,0)
};


double CGALCalculation::getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3){
    Triangle_3 tri(p1,p2,p3);
    return tri.squared_area();
}

Vector_3 CGALCalculation::getNormal(Vertex* va, Vertex* vb, Vertex* vc){
    Point_3 p3a(va->x,va->y,va->z);
    Point_3 p3b(vb->x,vb->y,vb->z);
    Point_3 p3c(vc->x,vc->y,vc->z);

    if (CGAL::collinear(p3a, p3b, p3c)){
        return CGAL::NULL_VECTOR;
    }
    else{
        return CGAL::unit_normal(p3a,p3b,p3c);
    }
}

double CGALCalculation::getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc){
    Point_3 p3a(va->x,va->y,va->z);
    Point_3 p3b(vb->x,vb->y,vb->z);
    Point_3 p3c(vc->x,vc->y,vc->z);
    return getSquaredArea(p3a, p3b, p3c);
}
