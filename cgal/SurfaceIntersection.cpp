//
// Created by dongmin on 18. 1. 29.
//

#include <compute/SurfaceComputation.h>
#include <compute/VertexListComputation.h>
#include "SurfaceIntersection.h"
#include "PolygonComputation.h"
#include <bitset>

using namespace std;

std::vector<Surface *> SurfaceIntersection::resolveSelfIntersection(Surface * &pSurface) {
    SurfaceIntersection::resolveEasySelfIntersection(pSurface);

    vector<Surface*> newSurfaceList;

    while (true){
        if (!makeNewIntersectionVertex(pSurface)) {
            break;
        }
    }




    return newSurfaceList;
}

bool SurfaceIntersection::makeNewIntersectionVertex(Surface *&pSurface){
    vector<Point_2> pointsList = SurfaceComputation::to2D(pSurface, pSurface->planeRef);
    vector<Segment_2> segmentList = VertexListComputation::makeSegment2List(pointsList);
    for (int i = 0 ; i < segmentList.size() - 2; i++){
        for (int j = i + 2 ; j < segmentList.size() ; j++){
            if (i == 0 && j == segmentList.size() - 1) continue;
            if (CGAL::do_intersect(segmentList[i], segmentList[j])){
                CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
                        result = CGAL::intersection(segmentList[i], segmentList[j]);
                if (result){
                    if (const Point_2* p = boost::get<Point_2>(&*result)){
                        int pi = 0;
                        Point_3 point3 = pSurface->planeRef.to_3d(*p);
                        Vertex* vt = new Vertex(point3.x(), point3.y(), point3.z());
                        for (;pi < 2 ; pi++){
                            double distI = CGAL::squared_distance(*p, segmentList[i][pi]);
                            if (distI < Checker::squaredDistanceOfSamePoint2D){
                                break;
                            }
                        }
                        int pj = 0;
                        for (;pj < 2 ; pj++){
                            double distJ = CGAL::squared_distance(*p, segmentList[j][pj]);
                            if (distJ < Checker::squaredDistanceOfSamePoint2D){
                                break;
                            }
                        }
                        if (pi != 2 && pj != 2){
                            int pointI = pi + i == segmentList.size() ? 0 : pi + i;
                            int pointJ = pj + j == segmentList.size() ? 0 : pj + j;
                            if (pSurface->getVertex(pointI) != pSurface->getVertex(pointJ)){
                                pSurface->setVertex(pointI, vt);
                                pSurface->setVertex(pointJ, vt);
                            }
                            continue;
                        }

                        // put new Vertex. i < j 이므로 j 부터 넣기.
                        if (pj == 2){
                            int insertIndex = j + 1;
                            pSurface->insertVertex(insertIndex, vt);
                        }

                        if (pi == 2){ // put Vertex
                            int insertIndex = i + 1;
                            pSurface->insertVertex(insertIndex, vt);
                        }

                        if (pi == 2 || pj == 2){
                            return true;
                        }

                    }
                    else if (const Segment_2* seg = boost::get<Segment_2>(&*result)){
                        Vector_2 segVector = seg->to_vector();
                        Vector_2 iVector = segmentList[i].to_vector();
                        cout << CGALCalculation::getAngle(segVector, iVector) << endl; continue;


                        Point_3 source = pSurface->planeRef.to_3d(seg->source());
                        Point_3 target = pSurface->planeRef.to_3d(seg->target());
                        Vertex* sourceVt = new Vertex(source.x(), source.y(), source.z());
                        Vertex* targetVt = new Vertex(target.x(), target.y(), target.z());

                        int pi = 0, pj = 0;

                        // Source
                        for (;pi < 2 ; pi++){
                            double distI = CGAL::squared_distance(seg->vertex(0), segmentList[i][pi]);
                            if (distI < Checker::squaredDistanceOfSamePoint2D){
                                break;
                            }
                        }

                        for (;pj < 2 ; pj++){
                            double distJ = CGAL::squared_distance(seg->vertex(0), segmentList[j][pj]);
                            if (distJ < Checker::squaredDistanceOfSamePoint2D){
                                break;
                            }
                        }

                        if (pj == 0){
                            double distJ = CGAL::squared_distance(seg->vertex(0), segmentList[j][pj]);
                            double distJ2 = CGAL::squared_distance(seg->vertex(0), segmentList[j][1]);
                            cout << "segment order";
                        }

                        if (pi != 2 && pj != 2) {
                            int pointI = pi + i == segmentList.size() ? 0 : pi + i;
                            int pointJ = pj + j == segmentList.size() ? 0 : pj + j;
                            if (pSurface->getVertex(pointI) != pSurface->getVertex(pointJ)){
                                pSurface->setVertex(pointI, sourceVt);
                                pSurface->setVertex(pointJ, sourceVt);
                            }
                        }
                        else{
                            if (pj == 2){

                            }
                            if (pi == 2){
                            }
                        }


                        // Target



                    }
                }
            }
        }
    }
    return false;
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