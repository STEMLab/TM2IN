//
// Created by dongmin on 18. 1. 12.
//

#ifndef TRIANGLEMESHCLEANER_HALFEDGE_H
#define TRIANGLEMESHCLEANER_HALFEDGE_H

#include "features/Triangle.h"

class HalfEdge {
public:
    std::vector<Vertex*> vertices;
    Triangle* parentTriangle = NULL;
    HalfEdge* oppositeEdge = NULL;

    HalfEdge(){}
    HalfEdge(Vertex* pa, Vertex* pb){
        vertices.push_back(pa);
        vertices.push_back(pb);
    }
    HalfEdge(Vertex* pa, Vertex* pb, Triangle* pTriangle){
        vertices.push_back(pa);
        vertices.push_back(pb);
        parentTriangle = pTriangle;
    }

    Triangle *getParentTriangle() const;

    void setParentTriangle(Triangle *parentTriangle);

    HalfEdge *getOppositeEdge() const;

    void setOppositeEdge(HalfEdge *oppositeEdge);

    bool isSame(HalfEdge*);
    bool isOpposite(HalfEdge*);

    Vertex* operator[](int idx){
        if (idx == 0 || idx == 1) return vertices[idx];
        else{
            std::cerr << "HalfEdge only has two vertices" << std::endl;
            exit(-1);
        }
    };
};


#endif //TRIANGLEMESHCLEANER_HALFEDGE_H
