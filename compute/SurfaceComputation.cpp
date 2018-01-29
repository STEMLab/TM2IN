//
// Created by dongmin on 18. 1. 18.
//

#include "SurfaceComputation.h"
#include "VertexComputation.h"
#include "compute/VertexListComputation.h"
#include "HalfEdgeComputation.h"


void SurfaceComputation::flatten(Surface *&sf) {
    Plane_3 plane = getPlane3(sf);

    vector<Vertex*> newVertices;
    for (ull index = 0 ; index < sf->sizeOfVertices() ; index++ )
    {
        Point_3 point = VertexComputation::getCGALPoint(sf->vertex(index));
        Point_3 projected = plane.projection(point);

        Vertex* v = new Vertex(projected.x(), projected.y(), projected.z());
        newVertices.push_back(v);
    }

    assert(newVertices.size() == sf->sizeOfVertices());
    sf->setVertices(newVertices);
    sf->planeRef = plane;
    sf->av_normal = plane.orthogonal_vector();
}

Vertex* SurfaceComputation::getCenterPoint(Surface *pSurface) {
    Vertex* center = VertexListComputation::getCenter(pSurface->getVerticesList());
    return center;
}

Plane_3 SurfaceComputation::getPlane3(Surface *&pSurface) {
    return VertexListComputation::getPlane3WithPCA(pSurface->getVerticesList());
}

int SurfaceComputation::intersectionCount = 0;

void SurfaceComputation::resolveIntersectionByCGAL(Surface *&pSurface) {
    bool hasIntersection = true;
    while (hasIntersection){
        hasIntersection = false;
        vector<HalfEdge* > edges = pSurface->getboundaryEdgesList();
        vector<Segment_3> segments;
        for (int i = 0 ; i < edges.size() ; i++){
            segments.push_back(HalfEdgeComputation::getCGALSegment_3(edges[i]));
            delete edges[i];
        }

        for (int i = 0 ; i < segments.size() - 2; i++){
            for (int j = i + 2 ; j < segments.size() ; j++){
                if (i == 0 && j == segments.size() - 1) continue;
                if (CGAL::do_intersect(segments[i], segments[j])){
                    CGAL::cpp11::result_of<Intersect_3(Segment_3, Segment_3)>::type
                            result = CGAL::intersection(segments[i], segments[j]);
//                    if (result){
//                        if (const Point_3* p = boost::get<Point_3>(&*result)){
//                            std::cout << *p << endl;
//                        }
//                    }
                    intersectionCount++;
                    hasIntersection = true;

                    // remove Vertex.
                    pSurface->removeVertexByIndex(i + 1);
                    i = segments.size();
                    break;
                }
            }
        }
        //if (hasIntersection) return true;
    }
    return;
}

Surface * SurfaceComputation::resolveIntersectionAndMakeNewSurface(Surface *&pSurface) {
    bool hasIntersect = true;
    while (hasIntersect){
        vector<Vertex*> vertexList = pSurface->getVerticesList();
        hasIntersect = false;
        for (int i = 0 ; i < vertexList.size() - 2 ; i++){
            for (int j = i + 1; j < vertexList.size() ; j++){
                if (vertexList[i] == vertexList[j]){
                    if ( j - i == 1){
                        //erase j
                        pSurface->removeVertexByIndex(j);
                    }
                    else if (j - i == 2){
                        //erase i + 1, j
                        pSurface->removeVertexByIndex(j);
                        pSurface->removeVertexByIndex(i+1);
                    }
                    else if (j - i > 2){
                        vector<Vertex*> newVertices;
                        for (int id = j ; id > i; id--){
                            newVertices.push_back(vertexList[id]);
                            pSurface->removeVertexByIndex(id);
                        }
                        Surface* newSurface = new Surface(newVertices);
                        return newSurface;
                    }
                    else{
                        exit(-1);
                    }
                    hasIntersect = true;
                    j = vertexList.size();
                    i = j;
                }
                else if (Checker::isSameVertex(vertexList[i], vertexList[j])){
                    cout << i <<" " <<  j << endl;
                    cout << vertexList[i]->toJSON() << endl;
                    cout << vertexList[j]->toJSON() << endl;
                    if ( j - i == 1){
                        //erase j
                        pSurface->removeVertexByIndex(j);
                    }
                    else if (j - i == 2){
                        //erase i + 1, j
                        pSurface->removeVertexByIndex(j);
                        pSurface->removeVertexByIndex(i+1);
                    }
                    else if (j - i > 2){
                        vector<Vertex*> newVertices;
                        for (int id = j ; id > i; id--){
                            newVertices.push_back(vertexList[id]);
                            pSurface->removeVertexByIndex(id);
                        }
                        Surface* newSurface = new Surface(newVertices);
                        return newSurface;
                    }
                    else{
                        exit(-1);
                    }
                    hasIntersect = true;
                    j = vertexList.size();
                    i = j;
                }
            }
        }
    }
    return NULL;
}


void SurfaceComputation::snapping(Surface *&pSurface) {
    std::vector<Vertex*> vertexList = pSurface->getVerticesList();
    vector<HalfEdge* > edges = pSurface->getboundaryEdgesList();
    vector<Segment_3> segments;
    for (int i = 0 ; i < edges.size() ; i++){
        segments.push_back(HalfEdgeComputation::getCGALSegment_3(edges[i]));
        delete edges[i];
    }

    for (int i = 0 ; i < vertexList.size() ; i++){
        int frontEdgeIndex = i - 1 >= 0? i-1: vertexList.size() - 1;
        int nextEdgeIndex = i;
        for (int ed = 0 ; ed < segments.size() ; ed++){
            if (ed == frontEdgeIndex || ed == nextEdgeIndex) continue;
            Point_3 point3 = VertexComputation::getCGALPoint(vertexList[i]);
            if (CGAL::squared_distance(point3, segments[ed]) < pow(Checker::threshold_vertex, 2)){
                Point_3 projected = segments[ed].supporting_line().projection(point3);
                vertexList[i]->setCoords(projected.x(), projected.y(), projected.z());
                break;
            }
        }

    }
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


void SurfaceComputation::triangulate(Surface *&pSurface) {
    std::vector<Vertex*> vertexList = pSurface->getVerticesList();
    vector<pair<D_Point, int > > points;
    for (int i = 0 ; i < vertexList.size() ; i++){
        Plane_3 pl = pSurface->planeRef;
        Point_2 point2d = pl.to_2d(VertexComputation::getCGALPoint(vertexList[i]));
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
        Delaunay::Face_handle face = fit;

        for (int i = 0 ; i < 3 ; i++){
            localTemp.push_back(vertexList[face->vertex(i)->info()]);
        }

        triangles.push_back(new Triangle(localTemp));
    }

    pSurface->triangles = triangles;

    return;
}
