//
// Created by dongmin on 18. 7. 30.
//

#include "PolygonMesh.h"

#include "features/Wall/Polygon.h"
#include "TriangleMesh.h"
#include "util.h"

namespace TM2IN {
    namespace RoomBoundary {
        void PolygonMesh::tag_ID(string name) {
            for (ull i = 0; i < (ull) polygons.size(); i++) {
                polygons[i]->geom_id = name + "_" + to_string(i);
            }
        }

        TriangleMesh *PolygonMesh::to_triangle_mesh() {
            vector<Triangle*> triangles;
            for (ull i = 0 ; i < polygons.size() ; i++){
                vector<Triangle*> p_triangles = polygons[i]->getTriangulation();
                triangles.insert(triangles.end(), p_triangles.begin(), p_triangles.end());
            }
            return new TriangleMesh(triangles);
        }

        std::string PolygonMesh::asJsonText() {
            throw std::runtime_error("PolygonMesh::asJsonText() not implemented");
        }

        void PolygonMesh::updateMBB() {
            throw std::runtime_error("PolygonMesh::updateMBB() not implemented");
        }
    }
}