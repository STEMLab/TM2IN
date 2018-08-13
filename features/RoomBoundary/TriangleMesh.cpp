//
// Created by dongmin on 18. 7. 25.
//

#include <queue>
#include "util.h"
#include "features/HalfEdge.h"
#include "features/Wall/Triangle.h"
#include "TriangleMesh.h"

namespace TM2IN{
    namespace RoomBoundary {
        TriangleMesh::TriangleMesh(std::vector<Triangle *> _triangles) {
            this->triangles = _triangles;
            this->type=IND_TYPE ::RoomBoundary;
        }

        void TriangleMesh::makeGraph() {
            std::vector <Triangle *> &triangleList = this->triangles;
            for (ull i = 0; i < triangleList.size() - 1; i++) {
                if (i % 10 == 0) printProcess(i, triangleList.size(), "make Graph");
                for (ull j = i + 1; j < triangleList.size(); j++) {
                    if (triangleList[i]->checkNeighbor(triangleList[j])) {
                        if (!triangleList[i]->isOpposite(triangleList[j])) {
                        } else {
                            throw std::runtime_error("Opposite Triangle");
                        }
                    }
                }
            }
            return;
        }

        bool TriangleMesh::is_closed_triangle_mesh() {
//            for (std::map<Triangle*, std::vector<Triangle*>>::iterator it=this->adjMap.begin() ; it != this->adjMap.end(); it++){
//                if (it->second.size() != 3) return false;
//            }
            return true;
        }

        std::vector<TriangleMesh *> TriangleMesh::disjoint_meshes() {
            std::vector<TriangleMesh*> connectedComponent;
            std::map<Triangle*, bool> checked;
            for (int i = 0 ; i < this->triangles.size() ; i++){
                checked[triangles[i]] = false;
            }

            for (ull i = 0; i < this->triangles.size(); i++) {
                if (!checked[triangles[i]]) {
                    checked[triangles[i]] = true;
                    TriangleMesh* tm = this->bfs(this->triangles[i], checked);
                    connectedComponent.push_back(tm);
                }
            }
            return connectedComponent;
        }

        TriangleMesh *TriangleMesh::bfs(Wall::Triangle *&pTriangle, std::map<Wall::Triangle *, bool> &checked) {
            std::vector<Triangle*> _triangles;
            std::queue<Triangle*> wait_queue;
            wait_queue.push(pTriangle);
            while (wait_queue.size() > 0) {
                Wall::Triangle* curr = wait_queue.front();
                wait_queue.pop();
                _triangles.push_back(curr);

                for (unsigned int nb = 0; nb < curr->getVerticesSize(); nb++) {
                    Wall::Triangle * next_surface = (Wall::Triangle*)(curr->exterior_boundary_edge(nb)->getOppositeEdge()->parent);
                    if (next_surface == NULL) throw std::runtime_error("bfs wrong in tm");
                    if (checked[next_surface]) continue;
                    else {
                        checked[next_surface] = true;
                        wait_queue.push(next_surface);
                    }
                }
            }
            TriangleMesh *tm = new TriangleMesh(_triangles);
            return tm;
        }

        std::vector<Triangle *> TriangleMesh::getTriangleList() {
            return this->triangles;
        }

        void TriangleMesh::updateMBB() {
            std::vector<Triangle_3> cgal_triangles;
            for (int i = 0 ; i < this->triangles.size() ; i++){
                cgal_triangles.push_back(this->triangles[i]->CGAL_triangle());
            }

            CGAL::Bbox_3 bbox_3 = CGAL::bbox_3(cgal_triangles.begin(), cgal_triangles.end());
            this->mbb->set(bbox_3);
        }

        std::string TriangleMesh::asJsonText() {
            throw std::runtime_error("TriangleMesh::asJsonText() not implemented");
        }

        std::vector<Vertex *> TriangleMesh::extractVerticesList() {
            std::vector<Vertex*> results;
            for (int i = 0 ; i < this->triangles.size() ; i++){
                std::vector<Vertex*> triangle_vertices = this->triangles[i]->getVerticesList();
                for (int ti = 0 ; ti < triangle_vertices.size() ; ti++){
                    bool found = false;
                    for (int j = 0 ; j < results.size() ; j++){
                        if (triangle_vertices[ti] == results[j])
                            found = true;
                    }
                    if (!found){
                        results.push_back(triangle_vertices[ti]);
                    }
                }
            }
            return results;
        }
    }

}