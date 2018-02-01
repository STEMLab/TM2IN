//
// Created by dongmin on 18. 1. 18.
//

#include "VertexComputation.h"
#include "VertexListComputation.h"
#include "features/HalfEdge.h"

void VertexListComputation::deleteVertexList(vector<Vertex*>& vertices){
    for (ull i = 0 ; i < vertices.size() ; i++){
        delete vertices[i];
    }
    vertices.clear();
}

Vertex *VertexListComputation::getCenter(vector<Vertex *> vertices) {
    double x = 0,y = 0,z = 0;
    for (ull i = 0 ; i < vertices.size() ; i++){
        x += vertices[i]->x();
        y += vertices[i]->y();
        z += vertices[i]->z();
    }
    Vertex* newVertex = new Vertex(x/ vertices.size(), y / vertices.size(), z / vertices.size());
    return newVertex;
}

Plane_3 VertexListComputation::getPlane3WithPCA(vector<Vertex *> vertices) {
    vector<Point_3> points;
    for (int i = 0 ; i < vertices.size() ; i++){
        points.push_back(VertexComputation::getCGALPoint(vertices[i]));
    }
    Plane_3 plane;
    linear_least_squares_fitting_3(points.begin(), points.end(), plane, CGAL::Dimension_tag<0>());
    return plane;
}

vector<HalfEdge *> VertexListComputation::makeHalfEdgesList(vector<Vertex *> vertices) {
    vector<HalfEdge*> edges;
    for (int i = 0 ; i < vertices.size() - 1 ; i++){
        HalfEdge* he = new HalfEdge(vertices[i], vertices[i+1]);
        edges.push_back(he);
    }
    edges.push_back(new HalfEdge(vertices[vertices.size() - 1], vertices[0]));
    return edges;
}

vector<Segment_2> VertexListComputation::makeSegment2List(vector<Point_2> &pointsList) {
    vector<Segment_2> segmentsList;
    for (int i = 0 ; i < pointsList.size() - 1 ; i++){
        Segment_2 seg(pointsList[i], pointsList[i+1]);
        segmentsList.push_back(seg);
    }
    Segment_2 last_seg(pointsList[pointsList.size() - 1], pointsList[0]);
    segmentsList.push_back(last_seg);
    return segmentsList;
}
