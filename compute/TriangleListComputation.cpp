//
// Created by dongmin on 18. 4. 4.
//
#include "TriangleListComputation.h"
#include "features/Triangle.h"

void TMIC::connectOppositeHalfEdges(std::vector <Triangle *>& triangles) {
    for (int i = 0 ; i < triangles.size() - 1 ; i++){
        if (i % 10 == 0) printProcess(i, triangles.size(), "connect Edges");
        for (int j = i + 1; j < triangles.size() ; j++){
            triangles[i]->setNeighbor(triangles[j]);
        }
    }
}
