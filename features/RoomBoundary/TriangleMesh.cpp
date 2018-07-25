//
// Created by dongmin on 18. 7. 25.
//

#include "util.h"
#include "features/Triangle.h"

#include "TriangleMesh.h"

namespace TM2IN{
    namespace RoomBoundary {
        TriangleMesh::TriangleMesh(std::vector<Triangle *> _triangles) {
            this->triangles = _triangles;
        }

        void TriangleMesh::makeGraph() {
            std::vector <Triangle *> &triangleList = this->triangles;
            for (ull i = 0; i < triangleList.size() - 1; i++) {
                if (i % 10 == 0) printProcess(i, triangleList.size(), "make Graph");
                for (ull j = i + 1; j < triangleList.size(); j++) {
                    if (triangleList[i]->checkNeighbor(triangleList[j])) {
                        if (!triangleList[i]->isOpposite(triangleList[j])) {
                            adjMap[triangleList[i]].push_back(triangleList[j]);
                            adjMap[triangleList[j]].push_back(triangleList[i]);
                        } else {
                            throw std::runtime_error("Opposite Triangle");
                        }
                    }
                }
            }
            return ;
        }

        bool TriangleMesh::is_closed_triangle_mesh() {
            return false;
        }
    }

}