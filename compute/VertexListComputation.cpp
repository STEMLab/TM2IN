//
// Created by dongmin on 18. 1. 18.
//

#include "VertexListComputation.h"

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
    Vertex* newVertex = new Vertex(x, y, z);
    return newVertex;
}
