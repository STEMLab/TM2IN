//
// Created by dongmin on 18. 7. 25.
//

#ifndef TM2IN_TRIANGLEMESH_H
#define TM2IN_TRIANGLEMESH_H

#include <features/IndoorComponent.h>

namespace TM2IN{
    namespace RoomBoundary{
        class TriangleMesh : public IndoorComponent {
        private:
            std::vector<Triangle*> triangles;
            TriangleMesh *bfs(Triangle *&pTriangle, std::map<Triangle *, bool> &checked);
        public:
            TriangleMesh(std::vector<Triangle *> _triangles);
            void makeGraph();
            bool is_closed_triangle_mesh();

            std::vector<TriangleMesh *> disjoint_meshes();
            std::vector<Triangle*> getTriangleList();

            std::vector<Vertex*> extractVerticesList();

            std::string asJsonText() override;

            void updateMBB() override;
        };
    }
}


#endif //TM2IN_TRIANGLEMESH_H
