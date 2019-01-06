//
// Created by dongmin on 18. 7. 30.
//

#include "util.h"
#include "TriangleMesh.h"

#include "TriangulatedSurfaceMesh.h"
#include "features/Wall/TriangulatedSurface.h"
#include "features/HalfEdge.h"
#include <queue>
#include <vector>
#include <detail/cgal/vector_3.h>

using namespace std;

namespace TM2IN{
    namespace RoomBoundary{
        TriangulatedSurfaceMesh::TriangulatedSurfaceMesh(TM2IN::RoomBoundary::TriangleMesh *tm) {
            vector<Triangle*> triangles = tm->getTriangleList();
            ull size = triangles.size();
            for (ull i = 0; i < size; i++) {
                Wall::TriangulatedSurface* newcp = new Wall::TriangulatedSurface(*triangles[i]);
                surfaces.push_back(newcp);
            }
            this->type=IND_TYPE ::RoomBoundary;
        }

        void TriangulatedSurfaceMesh::setSurfacesList(vector<Wall::TriangulatedSurface *> s_list) {
            this->surfaces = s_list;
        }

        vector<Wall::TriangulatedSurface *> TriangulatedSurfaceMesh::surface_list() {
            return this->surfaces;
        }

        int TriangulatedSurfaceMesh::surface_easy_validation() {
            cout << "\n------------- check whether surfaces are valid (easy) --------------\n" << endl;
            for (vector<TriangulatedSurface *>::size_type i = 0; i < this->surfaces.size();) {
                Wall::TriangulatedSurface * pSurface = this->surfaces[i];
                pSurface->updateMBB();

                if (pSurface->easy_validation()) {
                    i++;
                } else {
                    cerr << pSurface->asJsonText() << endl;
                    return -1;
                }
            }
            return 0;
        }

        int TriangulatedSurfaceMesh::update_surfaces_normal() {
            cout << "\n------------updateNormal------------\n" << endl;
            for (ull i = 0; i < (int) this->surfaces.size(); i++) {
                Wall::TriangulatedSurface* surface = this->surfaces[i];
                if (!surface->updateNormal()) {
                    cout << surface->asJsonText() << endl;
                    cout << "Cannot make Normal" << endl;
                    exit(-1);
                }

            }
            return 0;
        }

        ull TriangulatedSurfaceMesh::num_of_surfaces() {
            return (ull)this->surfaces.size();
        }

        bool TriangulatedSurfaceMesh::isClosed() {
            map<TriangulatedSurface *, bool> checked;
            for (TriangulatedSurface *sf : this->surfaces)
                checked[sf] = false;

            std::queue<TriangulatedSurface *> wait_queue;
            wait_queue.push(this->surfaces[0]);
            checked[this->surfaces[0]] = true;

            int surfaceCount = 0;

            while (wait_queue.size() > 0) {
                Wall::TriangulatedSurface * current = wait_queue.front();
                wait_queue.pop();

                surfaceCount += 1;
                for (unsigned int nb = 0; nb < current->getVerticesSize(); nb++) {
                    Wall::TriangulatedSurface* next_surface = (Wall::TriangulatedSurface*)current->exterior_boundary_edge(nb)->getOppositeEdge()->parent;
                    if (checked[next_surface]) continue;
                    else {
                        checked[next_surface] = true;
                        wait_queue.push(next_surface);
                    }
                }
            }

            if (surfaceCount != this->surfaces.size()) {
                return false;
            } else
                return true;
        }

        int TriangulatedSurfaceMesh::surface_strict_validation() {
            cout << "\n------------- check whether surfaces are valid --------------\n" << endl;
            for (vector<Wall::TriangulatedSurface *>::size_type i = 0; i < this->surfaces.size();) {
                Wall::TriangulatedSurface * pSurface = this->surfaces[i];
                pSurface->updateMBB();

                if (pSurface->strict_validation()) {
                    i++;
                } else {
                    cerr << pSurface->asJsonText() << endl;
                    return -1;
                }
            }
            return 0;
        }

        std::string TriangulatedSurfaceMesh::asJsonText() {
            throw std::runtime_error("TriangulatedSurfaceMesh::asJsonText() not implemented");
        }

        void TriangulatedSurfaceMesh::updateMBB() {
            throw std::runtime_error("TriangulatedSurfaceMesh::updateMBB not implemented");
        }

        void TriangulatedSurfaceMesh::classify_sf_type() {
            vector<Vector_3> normal_list = {
                    Vector_3(0,0,-1), // cs
                    Vector_3(0,0,1), // fs
                    Vector_3(1,0,0), // ws
                    Vector_3(0,1,0),
                    Vector_3(-1,0,0),
                    Vector_3(0,-1,0),
                    Vector_3(1,1,0),
                    Vector_3(1,-1,0),
                    Vector_3(-1,-1,0),
                    Vector_3(-1,1,0)
            };

            for (vector<Wall::TriangulatedSurface *>::size_type i = 0; i < this->surfaces.size(); i++) {
                Wall::TriangulatedSurface * pSurface = this->surfaces[i];
                Vector_3 nv = pSurface->normal;
                int type = TM2IN::detail::cgal::find_closest_vector(nv, normal_list);
                if (type == 0){
                    pSurface->surface_type = SURFACE_TYPE::CS;
                }
                else if (type == 1){
                    pSurface->surface_type = SURFACE_TYPE::FS;
                }
                else{
                    pSurface->surface_type = SURFACE_TYPE::WS;
                }

            }
        }


    }
}

