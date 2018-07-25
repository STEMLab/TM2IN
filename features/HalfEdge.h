//
// Created by dongmin on 18. 1. 12.
//

#ifndef TRIANGLEMESHCLEANER_HALFEDGE_H
#define TRIANGLEMESHCLEANER_HALFEDGE_H

#include "features/IndoorComponent.h"

namespace TM2IN {
    class HalfEdge : public IndoorComponent {
    private :

    public:
        std::vector<Vertex *> vertices;
        Surface *parent = NULL;
        HalfEdge *oppositeEdge = NULL;

        HalfEdge() {
        }

        HalfEdge(Vertex *pa, Vertex *pb, Surface *pSurface){
            type = TM2IN::IND_TYPE::HalfEdge;
            vertices.push_back(pa);
            vertices.push_back(pb);
            parent = pSurface;
        }

        Surface *getParent() const;

        void setParent(Surface *pSurface);

        HalfEdge *getOppositeEdge() const;

        void setOppositeEdge(HalfEdge *oppositeEdge);

        void setVertex(int i, Vertex *vt);

        bool isSame(HalfEdge *);

        bool hasOppositeTwoVertex(HalfEdge *);

        Vertex *operator[](int idx) {
            if (idx == 0 || idx == 1) return vertices[idx];
            else {
                std::cerr << "HalfEdge only has two vertices" << std::endl;
                exit(-1);
            }
        };

        Kernel::Segment_3 to_CGAL_segment();

        std::string asJsonText();
    };
}

#endif //TRIANGLEMESHCLEANER_HALFEDGE_H
