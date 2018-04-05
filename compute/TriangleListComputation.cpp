//
// Created by dongmin on 18. 4. 4.
//
#include "TriangleListComputation.h"
#include "features/Triangle.h"

void TMIC::connectOppositeHalfEdges(std::vector <Triangle *>& triangleList) {
    for (int i = 0 ; i < triangleList.size() - 1 ; i++){
        if (i % 10 == 0) printProcess(i, triangleList.size(), "connect Edges");
        for (int j = i + 1; j < triangleList.size() ; j++){

            if (CGALCalculation::isIntersect_BBOX(triangleList[i], triangleList[j])) triangleList[i]->setNeighbor(triangleList[j]);
        }
    }
}
