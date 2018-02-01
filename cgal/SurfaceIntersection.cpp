//
// Created by dongmin on 18. 1. 29.
//

#include <compute/SurfaceComputation.h>
#include <compute/VertexListComputation.h>
#include "SurfaceIntersection.h"
#include "PolygonComputation.h"

using namespace std;

std::vector<Surface *> SurfaceIntersection::resolveSelfIntersection(Surface * &pSurface) {
    SurfaceIntersection::resolveEasySelfIntersection(pSurface);

    vector<Surface*> newSurfaceList;
    vector<Point_2> pointsList = SurfaceComputation::to2D(pSurface, pSurface->planeRef);
    vector<Segment_2> segmentList = VertexListComputation::makeSegment2List(pointsList);

    for (int i = 0 ; i < segmentList.size() - 2; i++){
        for (int j = i + 2 ; j < segmentList.size() ; j++){
            if (i == 0 && j == segmentList.size() - 1) continue;
            if (CGAL::do_intersect(segmentList[i], segmentList[j])){
                cout << "intersect "<< i << ", " << j << endl;
                newSurfaceList.push_back(pSurface);
                return newSurfaceList;
            }
        }
    }

    return newSurfaceList;
}

#define GAP_FOR_SNAP 3

void SurfaceIntersection::resolveEasySelfIntersection(Surface *&pSurface) {
    vector<Point_2> pointsList = SurfaceComputation::to2D(pSurface, pSurface->planeRef);
    for (int i = 0 ; i < pointsList.size() - 1; i++){
        for (int j = 2 ; j <= GAP_FOR_SNAP ; j++){
            int nextIndex = i + j >= pointsList.size() ? i + j - pointsList.size() : i + j;
            Point_2 next = pointsList[nextIndex];
            if (CGAL::squared_distance(next, pointsList[i]) <= Checker::squaredDistanceOfSamePoint2D){
                if (nextIndex < i){
                    pointsList.erase(pointsList.begin() + i + 1, pointsList.end());
                    pointsList.erase(pointsList.begin(), pointsList.begin() + nextIndex + 1);

                    pSurface->removeVertexByIndex(i + 1, pSurface->getVerticesSize());
                    pSurface->removeVertexByIndex(0, nextIndex + 1);
                }
                else{
                    pointsList.erase(pointsList.begin() + i + 1, pointsList.begin() + nextIndex + 1);
                    pSurface->removeVertexByIndex(i + 1, nextIndex + 1);
                }
                break;
            }
        }
    }

    SurfaceComputation::removeStraight(pSurface);
}



/*
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
 */