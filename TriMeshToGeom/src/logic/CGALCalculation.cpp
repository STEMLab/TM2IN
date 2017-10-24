#include "logic/CGALCalculation.h"

#include "model/vertex.h"
#include "model/Segment.h"

double CGALCalculation::getAngle(Vector_3& nv1, Vector_3& nv2){
    double inner = (nv1 * nv2);
    double cos = inner / ( sqrt(nv1.squared_length()) * sqrt(nv2.squared_length()) );
    if (cos > 0.999999){
        return 0;
    }

    return acos(cos) * 180.0/PI;
}

Vector_3 CGALCalculation::getVector(Vertex* va, Vertex* vb){
    return Vector_3(va->getCGALPoint(),vb->getCGALPoint());
}

int CGALCalculation::findNormalType27(Vector_3& nv)
{

    int type = 0;
    double diff = 90.0;
    for (int i = 1 ; i < 27 ; i++){
        double temp_diff = CGALCalculation::getAngle(CGALCalculation::normal_list27[i], nv);
        if (temp_diff < diff){
            diff = temp_diff;
            type = i;
        }
    }
    return type;
}

int CGALCalculation::findNormalType18(Vector_3& nv)
{

    int type = 0;
    double diff = 90.0;
    for (int i = 0 ; i < 18 ; i++){
        double temp_diff = CGALCalculation::getAngle(CGALCalculation::normal_list18[i], nv);
        if (temp_diff < diff){
            diff = temp_diff;
            type = i;
        }
    }
    return type;
}

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


int CGALCalculation::findNormalType10(Vector_3& nv)
{
    int type = 0;
    double diff = 90.0;
    for (int i = 1 ; i < 11 ; i++){
        double temp_diff = CGALCalculation::getAngle(CGALCalculation::normal_list11[i], nv);
        if (temp_diff < diff){
            diff = temp_diff;
            type = i;
        }
    }
    return type;
}

Vector_3 CGALCalculation::normal_list18[18] = {
    Vector_3(0,0,1),
    Vector_3(0,0,-1),
    Vector_3(0,1,0),
    Vector_3(0,1,1),
    Vector_3(0,1,-1),
    Vector_3(0,-1,0),
    Vector_3(0,-1,1),
    Vector_3(0,-1,-1),
    Vector_3(1,0,0),
    Vector_3(1,0,1),
    Vector_3(1,0,-1),
    Vector_3(1,1,0),
    Vector_3(1,-1,0),
    Vector_3(-1,0,0),
    Vector_3(-1,0,1),
    Vector_3(-1,0,-1),
    Vector_3(-1,1,0),
    Vector_3(-1,-1,0)
};

Vector_3 CGALCalculation::normal_list27[27] = {
    Vector_3(0,0,0),
    Vector_3(0,0,1),
    Vector_3(0,0,-1),
    Vector_3(0,1,0),
    Vector_3(0,1,1),
    Vector_3(0,1,-1),
    Vector_3(0,-1,0),
    Vector_3(0,-1,1),
    Vector_3(0,-1,-1),
    Vector_3(1,0,0),
    Vector_3(1,0,1),
    Vector_3(1,0,-1),
    Vector_3(1,1,0),
    Vector_3(1,1,1),
    Vector_3(1,1,-1),
    Vector_3(1,-1,0),
    Vector_3(1,-1,1),
    Vector_3(1,-1,-1),
    Vector_3(-1,0,0),
    Vector_3(-1,0,1),
    Vector_3(-1,0,-1),
    Vector_3(-1,1,0),
    Vector_3(-1,1,1),
    Vector_3(-1,1,-1),
    Vector_3(-1,-1,0),
    Vector_3(-1,-1,1),
    Vector_3(-1,-1,-1)
};


Vector_3 CGALCalculation::normal_list11[11] = {
    Vector_3(0,0,0),
    Vector_3(0,0,1),
    Vector_3(0,0,-1),
    Vector_3(0,1,0),
    Vector_3(0,-1,0),
    Vector_3(1,0,0),
    Vector_3(1,1,0),
    Vector_3(1,-1,0),
    Vector_3(-1,0,0),
    Vector_3(-1,1,0),
    Vector_3(-1,-1,0)
};

Vector_3 CGALCalculation::normal_list6[6] = {
    Vector_3(1,0,0), //0, 3
    Vector_3(0,1,0),
    Vector_3(0,0,1),
    Vector_3(-1,0,0),
    Vector_3(0,-1,0),
    Vector_3(0,0,-1)
};


double CGALCalculation::getSquaredDistance(Vertex* v1, Vertex* v2){
    return CGAL::squared_distance(
                          v1->getCGALPoint(), v2->getCGALPoint());
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

bool CGALCalculation::isIntersect2D(Segment* seg1, Segment* seg2){
    return CGAL::do_intersect(seg1->getCGALSegmentWithoutZ(), seg2->getCGALSegmentWithoutZ());
}

Point_2 CGALCalculation::getIntersection2D(Segment* seg1, Segment* seg2){
    Segment_2 seg1_cgal = seg1->getCGALSegmentWithoutZ();
    Segment_2 seg2_cgal = seg2->getCGALSegmentWithoutZ();
    CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type result = intersection(seg1_cgal, seg2_cgal);
    Point_2* p = boost::get<Point_2 >(&*result);
    return *p;
}

bool CGALCalculation::isIntersect_BBOX(Surface* s1, Surface* s2){
    CGAL::Bbox_3 b1(s1->min_coords[0],s1->min_coords[1],s1->min_coords[2],s1->max_coords[0],s1->max_coords[1],s1->max_coords[2]);
    CGAL::Bbox_3 b2(s2->min_coords[0],s2->min_coords[1],s2->min_coords[2],s2->max_coords[0],s2->max_coords[1],s2->max_coords[2]);
    return CGAL::do_intersect(b1,b2);
}

//
//vector<vector<int>> CGALCalculation::triangulate(CSurface* sf){
//    vector<pair<Point, unsigned>> points;
//    for (int i = 0 ; i < (int)sf->v_list.size() ; i++){
//        points.push_back(make_pair(makeTPoint(sf->v_list[i]),i));
//    }
//
//    Delaunay triangulation;
//    triangulation.insert(points.begin(), points.end());
//
//    for(Delaunay::Finite_faces_iterator fit = triangulation.finite_faces_begin();
//    fit != triangulation.finite_faces_end(); ++fit)
//    {
//        Delaunay::Face_handle face = fit;
//        std::cout << "Triangle:\t" << triangulation.triangle(face) << std::endl;
//        std::cout << "Vertex 0:\t" << triangulation.triangle(face)[0] << std::endl;
//        std::cout << "Vertex 0:\t" << face->vertex(0)->info() << std::endl;
//    }
//
//}

//T_Point CGALCalculation::makeTPoint(Vertex* vt){
//    return T_Point(vt->x(), vt->y(), vt->z());
//}

