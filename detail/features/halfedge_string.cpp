//
// Created by dongmin on 18. 7. 24.
//

#include <detail/cgal/geometry.h>
#include "features/Triangle.h"
#include "features/HalfEdge.h"


#include "halfedge_string.h"

namespace TM2IN {
    namespace detail {
        namespace HalfEdgeString {
            void connectOppositeHalfEdges(std::vector <Triangle *>& triangleList) {
                for (int i = 0 ; i < triangleList.size() - 1 ; i++){
                    if (i % 10 == 0) printProcess(i, triangleList.size(), "connect Edges");
                    for (int j = i + 1; j < triangleList.size() ; j++){
                        if (TM2IN::detail::cgal::has_bbox_intersect(triangleList[i], triangleList[j]))
                            triangleList[i]->setNeighbor(triangleList[j]);
                    }
                }
            }

            void setParent(std::vector<HalfEdge *> edges, Surface *pSurface) {
                for (HalfEdge* edge : edges){
                    edge->setParent(pSurface);
                }
            }

            std::vector<Vertex *> getFirstVertexList(std::vector<HalfEdge *> halfEdges) {
                std::vector<Vertex*> vertexList;
                for (int i = 0 ; i < halfEdges.size() ; i++){
                    vertexList.push_back(halfEdges[i]->vertices[0]);
                }
                return vertexList;
            }


        }
    }
}