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