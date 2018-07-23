//
// Created by dongmin on 18. 1. 19.
//

#include "HalfEdgeComputation.h"
#include "VertexComputation.h"

using namespace std;

Line_3 HalfEdgeComputation::getCGALLine_3(HalfEdge *he) {
    Point_3 p1 = he->vertices[0]->CGAL_point();
    Point_3 p2 = he->vertices[1]->CGAL_point();
    Line_3 line(p1,p2);
    return line;
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
