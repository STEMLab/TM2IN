//
// Created by dongmin on 18. 7. 24.
//

#ifndef TM2IN_HALFEDGE_STRING_H
#define TM2IN_HALFEDGE_STRING_H

#include <vector>

class Triangle;
class Surface;
class HalfEdge;
class Vertex;

namespace TM2IN{
    namespace detail{
        namespace HalfEdgeString{
            void setParent(std::vector<HalfEdge *> edges, Surface *pSurface);
            std::vector<Vertex*> getFirstVertexList(std::vector<HalfEdge*> halfEdges);
            void connectOppositeHalfEdges(std::vector<Triangle*>& triangleList);
        }
    }
}
#endif //TM2IN_HALFEDGE_STRING_H
