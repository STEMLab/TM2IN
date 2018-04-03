//
// Created by dongmin on 18. 1. 19.
//

#include "HalfEdgeComputation.h"
#include "VertexComputation.h"

Line_3 HalfEdgeComputation::getCGALLine_3(HalfEdge *he) {
    Point_3 p1 = VertexComputation::getCGALPoint(he->vertices[0]);
    Point_3 p2 = VertexComputation::getCGALPoint(he->vertices[1]);
    Line_3 line(p1,p2);
    return line;
}

Segment_3 HalfEdgeComputation::getCGALSegment_3(HalfEdge *he){
    Point_3 p1 = VertexComputation::getCGALPoint(he->vertices[0]);
    Point_3 p2 = VertexComputation::getCGALPoint(he->vertices[1]);
    Segment_3 seg(p1, p2);
    return  seg;
}

void HalfEdgeComputation::setParent(std::vector<HalfEdge *> edges, Surface *pSurface) {
    for (HalfEdge* edge : edges){
        edge->setParent(pSurface);
    }
}
