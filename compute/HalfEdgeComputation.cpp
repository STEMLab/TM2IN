//
// Created by dongmin on 18. 1. 19.
//

#include <cgal/Features_to_CGAL_object.h>
#include "HalfEdgeComputation.h"
#include "VertexComputation.h"

using namespace std;

Line_3 HalfEdgeComputation::getCGALLine_3(HalfEdge *he) {
    Point_3 p1 = CGAL_User::getCGALPoint(he->vertices[0]);
    Point_3 p2 = CGAL_User::getCGALPoint(he->vertices[1]);
    Line_3 line(p1,p2);
    return line;
}

Segment_3 HalfEdgeComputation::getCGALSegment_3(HalfEdge *he){
    Point_3 p1 = CGAL_User::getCGALPoint(he->vertices[0]);
    Point_3 p2 = CGAL_User::getCGALPoint(he->vertices[1]);
    Segment_3 seg(p1, p2);
    return  seg;
}

void HalfEdgeComputation::setParent(std::vector<HalfEdge *> edges, Surface *pSurface) {
    for (HalfEdge* edge : edges){
        edge->setParent(pSurface);
    }
}

std::vector<Vertex *> HalfEdgeComputation::getFirstVertexList(std::vector<HalfEdge *> halfEdges) {
    vector<Vertex*> vertexList;
    for (int i = 0 ; i < halfEdges.size() ; i++){
        vertexList.push_back(halfEdges[i]->vertices[0]);
    }
    return vertexList;
}
