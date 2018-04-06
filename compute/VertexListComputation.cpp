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

bool VertexListComputation::checkDuplicate(vector<Vertex *> vertices) {
    vector<Vertex*> sorted_v_list(vertices);

    sort(sorted_v_list.begin(), sorted_v_list.end(), VertexComputation::greater);
    for (ull i = 0 ; i < sorted_v_list.size() - 1; i++){
        if (sorted_v_list[i] == sorted_v_list[i+1]){
            return true;
        }
    }
    return false;
}
