#include <compute/VertexComputation.h>
#include "CGALCalculation.h"

#include "features/Vertex.h"
#include "features/Surface.h"
#include "space_maker/OnlyWallSpaceMaker.h"

using namespace std;

double CGALCalculation::getAngle(Vector_3& nv1, Vector_3& nv2){
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

    return angle;
}


double CGALCalculation::getAngle(Vector_2& nv1, Vector_2& nv2){
    double nv1_length = sqrt(nv1.squared_length());
    double nv2_length = sqrt(nv2.squared_length());

    Vector_2 unit_nv1 = nv1 / nv1_length;
    Vector_2 unit_nv2 = nv2 / nv2_length;

    double inner = (unit_nv1 * unit_nv2);
    double cos = inner;
    //cout << " cos : " << cos <<endl;
    if (cos > 0.99999) cos = 1;
    else if (cos < -0.99999) cos = -1;
    double angle = acos(cos) * 180.0/PI;

    return angle;
}

Vector_3 CGALCalculation::getVector(Vertex* va, Vertex* vb){
    return Vector_3(VertexComputation::getCGALPoint(va),VertexComputation::getCGALPoint(vb));
}


double CGALCalculation::getSquaredDistance(Vertex* v1, Vertex* v2){
    return CGAL::squared_distance(
            VertexComputation::getCGALPoint(v1), VertexComputation::getCGALPoint(v2));
}

Vector_3 CGALCalculation::getUnitNormal(Vertex* va, Vertex* vb, Vertex* vc){
    Point_3 p3a(va->x(),va->y(),va->z());
    Point_3 p3b(vb->x(),vb->y(),vb->z());
    Point_3 p3c(vc->x(),vc->y(),vc->z());

    if (CGAL::collinear(p3a, p3b, p3c)){
        return CGAL::NULL_VECTOR;
    }
    else{
        return CGAL::unit_normal(p3a,p3b,p3c);
    }
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


bool CGALCalculation::isIntersect_BBOX(Surface* s1, Surface* s2){
    CGAL::Bbox_3 b1(s1->min_coords[0],s1->min_coords[1],s1->min_coords[2],s1->max_coords[0],s1->max_coords[1],s1->max_coords[2]);
    CGAL::Bbox_3 b2(s2->min_coords[0],s2->min_coords[1],s2->min_coords[2],s2->max_coords[0],s2->max_coords[1],s2->max_coords[2]);
    return CGAL::do_intersect(b1,b2);
}


bool CGALCalculation::isAngleLowerThan(Vector_3& nv1, Vector_3& nv2, double degree){
    double angle = CGALCalculation::getAngle(nv1, nv2);
    if (angle < 0) cerr << "angle is minus ?" << endl;
        /*
    if (degree < 0) return (angle >= 180 + degree);
    else
         */
        return (angle <= degree) ;
}


bool CGALCalculation::isAngleLowerThan(Vertex* origin, Vertex* v1, Vertex* v2, double degree){
    Point_3 p3a(origin->x(),origin->y(),origin->z());
    Point_3 p3b(v1->x(),v1->y(),v1->z());
    Point_3 p3c(v2->x(),v2->y(),v2->z());

    Vector_3 vec1(p3a,p3b);
    Vector_3 vec2(p3a,p3c);

    return CGALCalculation::isAngleLowerThan(vec1, vec2, degree);
}

//
//vector<vector<int>> CGALCalculation::triangulate(CSurface* sf){
//    vector<pair<Point, unsigned>> points;
//    for (int i = 0 ; i < (int)sf->getVerticesSize() ; i++){
//        points.push_back(make_pair(makeTPoint(sf->v_list[i]),i));
//    }
//
//    Delaunay globalIndicesOfTriangulation;
//    globalIndicesOfTriangulation.insert(points.begin(), points.end());
//
//    for(Delaunay::Finite_faces_iterator fit = globalIndicesOfTriangulation.finite_faces_begin();
//    fit != globalIndicesOfTriangulation.finite_faces_end(); ++fit)
//    {
//        Delaunay::Face_handle face = fit;
//        std::cout << "Triangle:\t" << globalIndicesOfTriangulation.triangle(face) << std::endl;
//        std::cout << "Vertex 0:\t" << globalIndicesOfTriangulation.triangle(face)[0] << std::endl;
//        std::cout << "Vertex 0:\t" << face->vertices(0)->info() << std::endl;
//    }
//
//}

//T_Point CGALCalculation::makeTPoint(Vertex* vt){
//    return T_Point(vt->x(), vt->y(), vt->z());
//}

