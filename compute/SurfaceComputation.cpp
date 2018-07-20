//
// Created by dongmin on 18. 1. 18.
//

#include <cgal/SurfaceIntersection.h>
#include <cgal/Features_to_CGAL_object.h>
#include <detail/feature/plane.h>
#include "SurfaceComputation.h"
#include "VertexComputation.h"
#include "compute/VertexListComputation.h"
#include "HalfEdgeComputation.h"
#include "features/TriangleMeshGraph.h"
#include "Connect_halfedges.h"

using namespace std;

void SurfaceComputation::removeConsecutiveDuplicationIndex(Surface *&pSurface){
    cerr << "TODO : SurfaceComputation::removeConsecutiveDuplicationIndex" << endl;
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
    Point_3 centerPoint = centerV->CGAL_point();
    Plane_3 plane(centerPoint, pSurface->normal);
    return plane;
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
    vector<Point_2> pointsList = TM2IN::detail::feature::project_to_plane(pSurface->getVerticesList(), plane3);
    vector<Segment_2> segmentsList;
    for (int i = 0 ; i < pointsList.size() - 1 ; i++){
        Segment_2 seg(pointsList[i], pointsList[i+1]);
        segmentsList.push_back(seg);
    }
    Segment_2 last_seg(pointsList[pointsList.size() - 1], pointsList[0]);
    segmentsList.push_back(last_seg);
    return segmentsList;
}
