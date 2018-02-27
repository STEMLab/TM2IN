//
// Created by dongmin on 18. 1. 18.
//

#include "SurfaceComputation.h"
#include "VertexComputation.h"
#include "compute/VertexListComputation.h"
#include "HalfEdgeComputation.h"
#include "cgal/PolygonComputation.h"

using namespace std;

void SurfaceComputation::removeConsecutiveDuplication(Surface *&pSurface){
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
}

void SurfaceComputation::removeConsecutiveDuplicationIndex(Surface *&pSurface){
    ull v_size = pSurface->v_list.size();
    int removed_count = 0;
    for (ull i = 0 ; i < v_size - 1; i++){
        /*
        if (Checker::isSameVertex(v_list[i] , v_list[i+1])){
            v_list.erase(v_list.begin() + i + 1);
            i--;
            v_size -= 1;
            removed_count += 1;
        }
        */
        if (pSurface->v_list[i] == pSurface->v_list[i+1]){
            pSurface->v_list.erase(pSurface->v_list.begin() + i + 1);
            i--;
            v_size -= 1;
            removed_count += 1;
        }
    }

    if (removed_count) cout << removed_count << " are removed in duplication" << endl;
}

void SurfaceComputation::flatten(Surface *&sf) {
    Plane_3 plane = makePlane3(sf);

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
    sf->setPlaneRef(plane);
    sf->normal = plane.orthogonal_vector();
}

Vertex* SurfaceComputation::getCenterPoint(Surface *pSurface) {
    Vertex* center = VertexListComputation::getCenter(pSurface->getVerticesList());
    return center;
}

Plane_3 SurfaceComputation::makePlane3(Surface *&pSurface) {
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

    if (removed_count) cout << removed_count << " are removed in straight" << endl;
}


int SurfaceComputation::triangulate(Surface *&pSurface) {
    std::vector<Vertex*> vertexList = pSurface->getVerticesList();

    // convert 3D point to 2D
    cout << "\n\n ===== to 2D ======" << endl;
    Plane_3 planeRef = SurfaceComputation::makePlane3(pSurface);
    pSurface->setPlaneRef(planeRef);
    vector<Point_2> point2dList = to2D(pSurface, planeRef);

    // partition Surface to convex 2D polygons.
    cout << "\n\n make Polygon" << endl;
    Polygon_2 polygon = PolygonComputation::makePolygon(point2dList);
    if (!polygon.is_simple()) return 1;

    cout << "\n\n partition Surface to convex polygons" << endl;
    vector<Polygon_2> polygonList = PolygonComputation::convexPartition(polygon);

    cout << "\n\n triangulate Polygons" << endl;
    vector<Triangle* > triangles;
    for (int i = 0 ; i < polygonList.size() ; i++){
        cout << polygonList[i] << endl;
        CGAL_assertion(polygonList[i].is_simple() && polygonList[i].is_convex());

        Polygon_2 p = polygonList[i];
        vector<Point_2> points;
        for (Polygon_2::Vertex_iterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi){
            Point_2 point2d(vi->x(), vi->y());
            points.push_back(point2d);
        }

        Delaunay T;
        T.insert(points.begin(),points.end());

        cout << "Triangles  : " << T.number_of_faces() << endl;
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

    pSurface->triangles = triangles;

    return 0;
}

std::vector<Segment_3> SurfaceComputation::makeSegment3List(Surface *&pSurface) {
    vector<Segment_3> result;
    vector<HalfEdge*> edges = SurfaceComputation::makeHalfEdgesList(pSurface);

    for (int i = 0 ; i < edges.size() ; i++){
        Segment_3  seg = HalfEdgeComputation::getCGALSegment_3(edges[i]);
        result.push_back(seg);
    }

    return result;
}

std::vector<HalfEdge *> SurfaceComputation::makeHalfEdgesList(Surface *&pSurface) {
    vector<HalfEdge*> edges;
    vector<Vertex*> vertices = pSurface->getVerticesList();
    for (int i = 0 ; i < vertices.size() - 1 ; i++){
        HalfEdge* he = new HalfEdge(vertices[i], vertices[i+1]);
        edges.push_back(he);
    }
    edges.push_back(new HalfEdge(vertices[vertices.size() - 1], vertices[0]));
    return edges;
}
