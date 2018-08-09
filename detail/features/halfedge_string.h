//
// Created by dongmin on 18. 7. 24.
//

#ifndef TM2IN_HALFEDGE_STRING_H
#define TM2IN_HALFEDGE_STRING_H

#include <vector>
#include "features/Triangle.h"

namespace TM2IN{
    namespace detail{
        namespace HalfEdgeString{
            /**
             * @ingroup imp_details
             * @brief Sets parents of all HalfEdge.
             */
            void setParent(std::vector<HalfEdge *> edges, Surface *pSurface);
            /**
             * @ingroup imp_details
             * @brief Returns a list of the start Vertex of each edges.
             */
            std::vector<Vertex*> getFirstVertexList(std::vector<HalfEdge*> halfEdges);
            /**
             * @ingroup imp_details
             * @brief Build opposite edge relation in a vector of Triangle
             */
            void connectOppositeHalfEdges(std::vector<Triangle*>& triangleList);
        }
    }
}
#endif //TM2IN_HALFEDGE_STRING_H
