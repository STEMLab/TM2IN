//
// Created by dongmin on 18. 1. 18.
//

#include <cgal/SurfaceIntersection.h>
#include <cgal/Features_to_CGAL_object.h>
#include "SurfaceComputation.h"
#include "VertexComputation.h"
#include "compute/VertexListComputation.h"
#include "HalfEdgeComputation.h"
#include "cgal/PolygonComputation.h"
#include "features/TriangleMeshGraph.h"
#include "Connect_halfedges.h"

using namespace std;

void SurfaceComputation::removeConsecutiveDuplication(Surface *&pSurface){
    /*
    ull v_size = pSurface->v_list.size();

    int removed_count = 0;
    for (ull i = 0 ; i < v_size - 1; i++){
        if (Checker::isSameVertex(pSurface->v_list[i] , pSurface->v_list[i+1])){
            pSurface->v_list.erase(pSurface->v_list.begin() + i + 1);
            i--;
            v_size -= 1;
            removed_count += 1;
        }
    }

    if (removed_count) cout << removed_count << " are removed in duplication" << endl;
    */
}

void SurfaceComputation::removeConsecutiveDuplicationIndex(Surface *&pSurface){
    cerr << "TODO : SurfaceComputation::removeConsecutiveDuplicationIndex" << endl;
    /*
    ull v_size = pSurface->v_list.size();
    int removed_count = 0;
    for (ull i = 0 ; i < v_size - 1; i++){
        if (pSurface->v_list[i] == pSurface->v_list[i+1]){
            pSurface->v_list.erase(pSurface->v_list.begin() + i + 1);
            i--;
            v_size -= 1;
            removed_count += 1;
        }
    }

    if (removed_count) cout << removed_count << " are removed in duplication" << endl;
     */
}

void SurfaceComputation::flatten(Surface *&sf) {
    Plane_3 plane = getPlane3WithPCA(sf);

    vector<Vertex*> newVertices;
    for (ull index = 0 ; index < sf->getVerticesSize() ; index++ )
    {
        Point_3 point = CGAL_User::getCGALPoint(sf->vertex(index));
        Point_3 projected = plane.projection(point);

        Vertex* v = new Vertex(projected.x(), projected.y(), projected.z());
        newVertices.push_back(v);
    }

    assert(newVertices.size() == sf->getVerticesSize());
    sf->setVertexList(newVertices);
    sf->setPlaneRef(plane);
    sf->normal = plane.orthogonal_vector();
}

Vertex* SurfaceComputation::getCenterPoint(Surface *pSurface) {
    Vertex* center = VertexListComputation::getCenter(pSurface->getVerticesList());
    return center;
}

Plane_3 SurfaceComputation::getPlane3WithMBB(Surface *&pSurface){
    pSurface->updateMBB();
    double mindist = 100000000;
    int minaxis = 0;
    for (int i = 0 ; i < 3 ; i ++){
        double dist = pSurface->max_coords[i] - pSurface->min_coords[i];
        if (dist < mindist){
            minaxis = i;
            mindist = dist;
        }
    }
    Vector_3 newNormal;
    assert(pSurface->normal[minaxis] != 0);
    switch (minaxis){
        case 0:
            newNormal = Vector_3(pSurface->normal[0],0,0);
            break;
        case 1:
            newNormal = Vector_3(0,pSurface->normal[1],0);
            break;
        case 2:
            newNormal = Vector_3(0,0,pSurface->normal[2]);
            break;
    }
    if (newNormal[minaxis] > 0){
        Point_3 point3(pSurface->min_coords[0], pSurface->min_coords[1], pSurface->min_coords[2]);
        Plane_3 plane3(point3, newNormal);
        return plane3;
    }
    else{
        Point_3 point3(pSurface->max_coords[0], pSurface->max_coords[1], pSurface->max_coords[2]);
        Plane_3 plane3(point3, newNormal);
        return plane3;
    }

}

Plane_3 SurfaceComputation::getPlane3WithCenter(Surface *&pSurface){
    Vertex* centerV = VertexListComputation::getCenter(pSurface->getVerticesList());
    Point_3 centerPoint = CGAL_User::getCGALPoint(centerV);
    Plane_3 plane(centerPoint, pSurface->normal);
    return plane;
}

Plane_3 SurfaceComputation::getPlane3WithPCA(Surface *&pSurface) {
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

std::vector<Point_2> SurfaceComputation::projectTo3DPlane(Surface *&pSurface, Plane_3 plane) {
    std::vector<Vertex*> vertexList = pSurface->getVerticesList();
    std::vector<Point_2> pointList;
    for (int i = 0 ; i < vertexList.size(); i++){
        Point_2 point2d = plane.to_2d(CGAL_User::getCGALPoint(vertexList[i]));
        pointList.push_back(point2d);
    }
    return pointList;
}


void SurfaceComputation::removeStraight(Surface*& pSurface){
    if (pSurface->getVerticesSize() < 3) return;
    int removed_count = 0;

    vector<Vertex*> vertexList = pSurface->getVerticesList();
    for (ll i = 0 ; i < vertexList.size(); ){
        if (vertexList.size() < 3) return;
        ll firstIndex = i == 0? vertexList.size() - 1: i-1;
        Vertex* start_p = vertexList[firstIndex];
        ll secondIndex = i;
        Vertex* check_p = vertexList[secondIndex];
        ll thirdIndex = i + 1 == vertexList.size()? 0 : i+1;
        Vertex* end_p = vertexList[thirdIndex];
        if (Checker::isCollinear(start_p, check_p, end_p)){
            vertexList.erase(vertexList.begin() + i);
            removed_count++;
            Checker::num_of_straight += 1;
        }
        else{
            i++;
        }
    }

    pSurface->setVertexList(vertexList);

    if (removed_count) cout << removed_count << " are removed in straight" << endl;
}


int SurfaceComputation::triangulate(Surface *&pSurface, std::vector<Triangle*>& result) {
    result.clear();

    std::vector<Vertex*> vertexList = pSurface->getVerticesList();
    if (vertexList.size() <= 4){
        pSurface->setNormal(pSurface->getSimpleNormal());
    }

    if (vertexList.size() == 3) {
        Triangle* newTriangle = new Triangle(vertexList);
        result.push_back(newTriangle);
        return 0;
//        pSurface->triangles.clear();
//        pSurface->triangles.push_back(newTriangle);
    }
    else{
        // convert 3D point to 2D
        Plane_3 planeRef = SurfaceComputation::getSimplePlane3WithNormal(pSurface->normal);
        vector<Point_2> point2dList = projectTo3DPlane(pSurface, planeRef);

        // partition Surface to convex 2D polygons.
        Polygon_2 polygon = PolygonComputation::makePolygon(point2dList);
        if (!polygon.is_simple())
        {
            cerr << "polygon is not simple" << endl;
            cerr << pSurface->toJSONString() << endl;
            cerr << polygon << endl;
            return 1;
        }
        if (polygon.orientation() == -1){
            cerr << polygon << endl;
            cerr << polygon.orientation() << endl;
            return 1;
        }
        vector<Polygon_2> polygonList = PolygonComputation::convexPartition(polygon);

        vector<Triangle* > triangles;
        for (int i = 0 ; i < polygonList.size() ; i++){
            CGAL_assertion(polygonList[i].is_simple() && polygonList[i].is_convex());

            Polygon_2 p = polygonList[i];
            vector<Point_2> points;
            for (Polygon_2::Vertex_iterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi){
                Point_2 point2d(vi->x(), vi->y());
                points.push_back(point2d);
            }

            Delaunay T;
            T.insert(points.begin(),points.end());
            for(Delaunay::Finite_faces_iterator fit = T.finite_faces_begin();
                fit != T.finite_faces_end(); ++fit)
            {
                vector<Vertex*> localTemp;
                Delaunay::Face_handle facet = fit;

                for (int j = 0 ; j < 3 ; j++){
                    Point_2 point2d = facet->vertex(j)->point();
                    int k;
                    for (k = 0 ; k < point2dList.size() ; k++){
                        if (point2d == point2dList[k]) break;
                    }
                    if (k == point2dList.size()){
                        cerr << "new Point" << endl;
                        exit(-1);
                    }
                    localTemp.push_back(vertexList[k]);
                }

                triangles.push_back(new Triangle(localTemp));
            }
        }
        result = triangles;
    }
    return 0;
}

std::vector<Segment_3> SurfaceComputation::makeSegment3List(Surface *&pSurface) {
    vector<Segment_3> result;
    vector<HalfEdge*> edges = pSurface->getBoundaryEdgesList();

    for (int i = 0 ; i < edges.size() ; i++){
        Segment_3  seg = HalfEdgeComputation::getCGALSegment_3(edges[i]);
        result.push_back(seg);
    }

    return result;
}

std::vector<Segment_2> SurfaceComputation::makeSegment2List(Surface *&pSurface, Plane_3 plane3) {
    vector<Point_2> pointsList = SurfaceComputation::projectTo3DPlane(pSurface, plane3);
    vector<Segment_2> segmentsList;
    for (int i = 0 ; i < pointsList.size() - 1 ; i++){
        Segment_2 seg(pointsList[i], pointsList[i+1]);
        segmentsList.push_back(seg);
    }
    Segment_2 last_seg(pointsList[pointsList.size() - 1], pointsList[0]);
    segmentsList.push_back(last_seg);
    return segmentsList;
}

Plane_3 SurfaceComputation::getSimplePlane3WithNormal(Vector_3 pNormal) {
    int type = CGALCalculation::findNormalType6(pNormal);
    Vector_3 normal = CGALCalculation::normal_list6[type];
    Point_3 origin(0,0,0);
    Plane_3 plane3(origin, normal);
    return plane3;
}

double TMIC::computeError(Surface *&pSurface) {
    double difference = 0.0;
    Plane_3 pca = SurfaceComputation::getPlane3WithPCA(pSurface);
    vector<Vertex*> vertices = pSurface->getVerticesList();
    for (Vertex* vt : vertices){
        Point_3 pt = vt->getCGALPoint();
        double dist = CGAL::squared_distance(pt, pca);
        if (dist > 0.000001)
            difference += CGAL::to_double(sqrt(dist));
    }

    return difference;
}
