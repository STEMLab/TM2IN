//
// Created by dongmin on 18. 1. 12.
//

#include "HalfEdge.h"

bool HalfEdge::isSame(HalfEdge* he){
    if (this->vertex[0] == he->vertex[0]){
        if (this->vertex[1] == he->vertex[1])
            return true;
    }
    return false;
}

Triangle *HalfEdge::getParentTriangle() const {
    return parentTriangle;
}

void HalfEdge::setParentTriangle(Triangle *parentTriangle) {
    HalfEdge::parentTriangle = parentTriangle;
}

HalfEdge *HalfEdge::getOppositeEdge() const {
    return oppositeEdge;
}

void HalfEdge::setOppositeEdge(HalfEdge *oppositeEdge) {
    HalfEdge::oppositeEdge = oppositeEdge;
}

bool HalfEdge::isOpposite(HalfEdge * he) {
    if (this->vertex[0] == he->vertex[1]){
        if (this->vertex[1] == he->vertex[0])
            return true;
    }
    return false;
}
