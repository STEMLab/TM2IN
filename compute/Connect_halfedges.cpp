//
// Created by dongmin on 18. 4. 4.
//
#include "Connect_halfedges.h"
#include "features/Triangle.h"
#include "features/HalfEdge.h"

void CGAL_User::connectOppositeHalfEdges(std::vector <Triangle *>& triangleList) {
    for (int i = 0 ; i < triangleList.size() - 1 ; i++){
        if (i % 10 == 0) printProcess(i, triangleList.size(), "connect Edges");
        for (int j = i + 1; j < triangleList.size() ; j++){

            if (CGALCalculation::isIntersect_BBOX(triangleList[i], triangleList[j])) triangleList[i]->setNeighbor(triangleList[j]);
        }
    }
}


