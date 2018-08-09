//
// Created by dongmin on 18. 1. 12.
//

#ifndef TRIANGLEMESHCLEANER_HALFEDGE_H
#define TRIANGLEMESHCLEANER_HALFEDGE_H

#include "features/IndoorComponent.h"

namespace TM2IN {
    /**
     * @ingroup geo_features
     */
    class HalfEdge : public IndoorComponent {
    private :

    public:
        std::vector<Vertex *> vertices;
        Surface *parent = NULL;
        HalfEdge *oppositeEdge = NULL;

        /**
         * @brief Empty HalfEdge constructor
         */
        HalfEdge() {}

        /**
         * @brief HalfEdge constructor
         */
        HalfEdge(Vertex *pa, Vertex *pb, Surface *pSurface){
            type = TM2IN::IND_TYPE::HalfEdge;
            vertices.push_back(pa);
            vertices.push_back(pb);
            parent = pSurface;
        }

        /**
         * @brief Returns parent Surface
         */
        Surface *getParent() const;

        /**
         * @brief Sets parent Surface
         */
        void setParent(Surface *pSurface);

        /**
         * @brief Returns opposite HalfEdge
         */
        HalfEdge *getOppositeEdge() const;

        /**
         * @brief Sets opposite HalfEdge
         */
        void setOppositeEdge(HalfEdge *oppositeEdge);

        /**
         * @brief Sets the i-th Vertex
         */
        void setVertex(int i, Vertex *vt);

        /**
         * @brief Checks whether it is same edge.
         */
        bool isSame(HalfEdge *);
        /**
         * @brief Checks whether the other has possible opposite edge.
         */
        bool can_be_opposite_edge(HalfEdge *);

        Vertex *operator[](int idx) {
            if (idx == 0 || idx == 1) return vertices[idx];
            else {
                std::cerr << "HalfEdge only has two vertices" << std::endl;
                exit(-1);
            }
        };

        /**
         *
         * @brief Converts to Kernel::Segment_3
         */
        Kernel::Segment_3 to_CGAL_segment();

        /**
         * @brief Returns json string
         */
        std::string asJsonText();
    };
}

#endif //TRIANGLEMESHCLEANER_HALFEDGE_H
