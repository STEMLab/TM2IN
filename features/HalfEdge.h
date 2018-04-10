//
// Created by dongmin on 18. 1. 12.
//

#ifndef TRIANGLEMESHCLEANER_HALFEDGE_H
#define TRIANGLEMESHCLEANER_HALFEDGE_H

#include "features/Triangle.h"

class HalfEdge {
private :

public:
    std::vector<Vertex*> vertices;
    Surface* parent = NULL;
    HalfEdge* oppositeEdge = NULL;

    HalfEdge(){}
    HalfEdge(Vertex* pa, Vertex* pb, Surface* pSurface){
        vertices.push_back(pa);
        vertices.push_back(pb);
        parent = pSurface;
    }

    Surface * getParent() const;

    void setParent(Surface *pSurface);

    HalfEdge *getOppositeEdge() const;

    void setOppositeEdge(HalfEdge *oppositeEdge);

    bool isSame(HalfEdge*);
    bool hasOppositeTwoVertex(HalfEdge *);

    Vertex* operator[](int idx){
        if (idx == 0 || idx == 1) return vertices[idx];
        else{
            std::cerr << "HalfEdge only has two vertices" << std::endl;
            exit(-1);
        }
    };
};


#endif //TRIANGLEMESHCLEANER_HALFEDGE_H
