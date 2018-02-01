//
// Created by dongmin on 18. 1. 18.
//

#include "SurfaceComputation.h"
#include "VertexComputation.h"
#include "compute/VertexListComputation.h"
#include "HalfEdgeComputation.h"
#include "cgal/PolygonComputation.h"

using namespace std;

void SurfaceComputation::flatten(Surface *&sf) {
    Plane_3 plane = getPlane3(sf);

    vector<Vertex*> newVertices;
    for (ull index = 0 ; index < sf->getVerticesSize() ; index++ )
    {
        Point_3 point = VertexComputation::getCGALPoint(sf->vertex(index));
        Point_3 projected = plane.projection(point);

        Vertex* v = new Vertex(projected.x(), projected.y(), projected.z());
        newVertices.push_back(v);
    }

    assert(newVertices.size() == sf->getVerticesSize());
    sf->setVertices(newVertices);
    sf->planeRef = plane;
    sf->normal = plane.orthogonal_vector();
}

Vertex* SurfaceComputation::getCenterPoint(Surface *pSurface) {
    Vertex* center = VertexListComputation::getCenter(pSurface->getVerticesList());
    return center;
}

Plane_3 SurfaceComputation::getPlane3(Surface *&pSurface) {
    Plane_3 plane = VertexListComputation::getPlane3WithPCA(pSurface->getVerticesList());
    Vector_3 normal = pSurface->normal;
    Vector_3 planeVector = plane.orthogonal_vector();
    if (pSurface->normal == CGAL::NULL_VECTOR) return plane;
    if (CGALCalculation::getAngle(planeVector, normal) > 90){
        return plane.opposite();
    }
    else
        return plane;
}

std::vector<Point_2> SurfaceComputation::to2D(Surface *&pSurface, Plane_3 plane) {
    std::vector<Vertex*> vertexList = pSurface->getVerticesList();
    std::vector<Point_2> pointList;
    for (int i = 0 ; i < vertexList.size() ; i++){
        Point_2 point2d = plane.to_2d(VertexComputation::getCGALPoint(vertexList[i]));
        pointList.push_back(point2d);
    }
    return pointList;
}


void SurfaceComputation::removeStraight(Surface*& pSurface){
    if (pSurface->getVerticesSize() < 3) return;
    int removed_count = 0;

    vector<Vertex*> vertexList = pSurface->getVerticesList();
    for (ll i = 0 ; i < vertexList.size(); ){
        ll firstIndex = i == 0? vertexList.size() - 1: i-1;
        Vertex* start_p = vertexList[firstIndex];
        ll secondIndex = i;
        Vertex* check_p = vertexList[secondIndex];
        ll thirdIndex = i + 1 == vertexList.size()? 0 : i+1;
        Vertex* end_p = vertexList[thirdIndex];
        if (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, Checker::degreeOfStraight)
            || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, Checker::degreeOfStraight)
            || CGALCalculation::isAngleLowerThan(check_p, end_p, start_p, Checker::degreeOfStraight)
                ){
            vertexList.erase(vertexList.begin() + i);
            removed_count++;
        }
        else{
            i++;
        }
    }

    pSurface->setVertices(vertexList);
/*
    vector<Vertex*> new_v_list;// = this->v_list;

    ll index = 1;
    Vertex* start_p = this->v_list[0];
    Vertex* check_p = this->v_list[index];

    do {
        ll next_index = index + 1;
        if (next_index == (ll) this->getVerticesSize()) next_index = 0;
        Vertex* end_p = this->v_list[next_index];
        if (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, degree)){
            removed_count++;
        }
        else{
            new_v_list.push_back(this->v_list[index]);
            start_p = this->v_list[index];
        }
        index = next_index;
        check_p = this->v_list[index];
    } while (index != 1);

    for (ull i = 1 ; i < new_v_list.size() - 1; ){
        Vertex* start_p = new_v_list[i-1];
        ull second = i;
        Vertex* check_p = new_v_list[second];
        ull third = i+1;
        Vertex* end_p = new_v_list[third];
        if (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, degree)
           || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, degree) ){
            new_v_list.erase(new_v_list.begin() + i);
        }
        else if(CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, -degree)
           || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, -degree) ){
            new_v_list.erase(new_v_list.begin() + i);
        }
        else{
            i++;
        }
    }

    this->v_list.clear();
    this->v_list = new_v_list;
*/
    if (removed_count) cout << removed_count << " are removed in straight" << endl;
}


void SurfaceComputation::triangulate(Surface *&pSurface) {
    std::vector<Vertex*> vertexList = pSurface->getVerticesList();

    // convert 3D point to 2D
    cout << "\n\n ===== to 2D ======" << endl;
    vector<Point_2> point2dList = to2D(pSurface, pSurface->planeRef);

    // partition Surface to convex 2D polygons.
    cout << "\n\n make Polygon" << endl;
    Polygon_2 polygon = PolygonComputation::makePolygon(point2dList);
    cout << "\n\n partition Surface to convex polygons" << endl;
    vector<Polygon_2> polygonList = PolygonComputation::convexPartition(polygon);

    cout << "\n\n triangulate Polygons" << endl;
    for (int i = 0 ; i < polygonList.size() ; i++){
        cout << polygonList[i] << endl;
        CGAL_assertion(polygonList[i].is_simple() && polygonList[i].is_convex());

        Polygon_2 p = polygonList[i];
        for (Polygon_2::Vertex_iterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi){
            std::cout << "vertex " << " = " << vi->x() << std::endl;
        }
    }

    return;

    vector<pair<D_Point, int > > points;
    for (int i = 0 ; i < vertexList.size() ; i++){
        Point_2 point2d = point2dList[i];
        points.push_back(make_pair(D_Point(point2d.x(), point2d.y()), i));
    }

    Delaunay T;
    T.insert(points.begin(),points.end());
    CGAL_assertion(T.number_of_vertices() == vertexList.size());

    vector<Triangle* > triangles;
    for(Delaunay::Finite_faces_iterator fit = T.finite_faces_begin();
        fit != T.finite_faces_end(); ++fit)
    {
        vector<Vertex*> localTemp;
        Delaunay::Face_handle facet = fit;

        for (int i = 0 ; i < 3 ; i++){
            localTemp.push_back(vertexList[facet->vertex(i)->info()]);
        }

        triangles.push_back(new Triangle(localTemp));
    }

    pSurface->triangles = triangles;

    return;
}
