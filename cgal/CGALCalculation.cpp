#include <compute/SurfaceComputation.h>
#include <compute/VertexComputation.h>
#include "CGALCalculation.h"

#include "features/Vertex.h"
#include "features/Surface.h"

using namespace std;

double CGALCalculation::getAngle(Vector_3& nv1, Vector_3& nv2){
    assert(nv1.squared_length() > 0 && nv2.squared_length() > 0);
    double nv1_length = sqrt(nv1.squared_length());
    double nv2_length = sqrt(nv2.squared_length());

    Vector_3 unit_nv1 = nv1 / nv1_length;
    Vector_3 unit_nv2 = nv2 / nv2_length;

    double inner = (unit_nv1 * unit_nv2);
    double cos = inner;
    //cout << " cos : " << cos <<endl;
    if (cos > 0.99999) cos = 1;
    else if (cos < -0.99999) cos = -1;
    double angle = acos(cos) * 180.0/PI;

    assert(angle == angle); //check NAN
    return angle;
}

Vector_3 CGALCalculation::getCrossProduct(Vertex* va, Vertex* vb, Vertex* vc){
    Point_3 p3a(va->x(),va->y(),va->z());
    Point_3 p3b(vb->x(),vb->y(),vb->z());
    Point_3 p3c(vc->x(),vc->y(),vc->z());

    Vector_3 vab(p3a, p3b);
    Vector_3 vac(p3a, p3c);

    return CGAL::cross_product(vab, vac);
}



double CGALCalculation::getSquaredArea(Vertex* va, Vertex* vb, Vertex* vc){
    Point_3 p3a(va->x(),va->y(),va->z());
    Point_3 p3b(vb->x(),vb->y(),vb->z());
    Point_3 p3c(vc->x(),vc->y(),vc->z());
    return getSquaredArea(p3a, p3b, p3c);
}

double CGALCalculation::getSquaredArea(Point_3& p1, Point_3& p2, Point_3& p3){
    Triangle_3 tri(p1,p2,p3);
    return tri.squared_area();
}



bool CGALCalculation::isAngleLowerThan(Vector_3& nv1, Vector_3& nv2, double degree){
    double angle = CGALCalculation::getAngle(nv1, nv2);
    assert(angle >= 0);
    return (angle <= degree) ;
}


bool CGALCalculation::isAngleLowerThan(Vertex* origin, Vertex* v1, Vertex* v2, double degree){
    Point_3 p3a(origin->x(),origin->y(),origin->z());
    Point_3 p3b(v1->x(),v1->y(),v1->z());
    Point_3 p3c(v2->x(),v2->y(),v2->z());

    Vector_3 vec1(p3a,p3b);
    Vector_3 vec2(p3a,p3c);
    assert(vec1.squared_length() > 0 && vec2.squared_length() > 0);

    return CGALCalculation::isAngleLowerThan(vec1, vec2, degree);
}


Vector_3 CGALCalculation::normal_list6[6] = {
        Vector_3(1,0,0), //0, 3
        Vector_3(0,1,0),
        Vector_3(0,0,1),
        Vector_3(-1,0,0),
        Vector_3(0,-1,0),
        Vector_3(0,0,-1)
};

int CGALCalculation::findNormalType6(Vector_3& nv)
{
    int type = 0;
    double diff = 90.0;
    for (int i = 0 ; i < 6 ; i++){
        double temp_diff = CGALCalculation::getAngle(CGALCalculation::normal_list6[i], nv);
        if (temp_diff < diff){
            diff = temp_diff;
            type = i;
        }
    }
    return type;
}

