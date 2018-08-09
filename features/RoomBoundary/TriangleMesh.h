//
// Created by dongmin on 18. 7. 25.
//

#ifndef TM2IN_TRIANGLEMESH_H
#define TM2IN_TRIANGLEMESH_H

#include <features/IndoorComponent.h>

namespace TM2IN{
    namespace RoomBoundary{
        /**
     * @ingroup geo_features
         * @brief Mesh consists of only TM2IN::Triangle
     */
        class TriangleMesh : public IndoorComponent {
        private:
            /**
             * @todo encapsulation
             */
            std::vector<Triangle*> triangles;
            TriangleMesh *bfs(Triangle *&pTriangle, std::map<Triangle *, bool> &checked);
        public:
            /**
             * @brief Constructor with a Triangle vector.
             */
            TriangleMesh(std::vector<Triangle *> _triangles);
            /**
             * @brief Makes Graph
             * @todo be removed
             */
            void makeGraph();
            /**
             *
             * @brief Checks mesh is closed
             */
            bool is_closed_triangle_mesh();

            /**
             * @brief Disjoint one mesh to multiple meshes by using connected component.
             */
            std::vector<TriangleMesh *> disjoint_meshes();

            /**
             * @brief Gets a list of all Triangle in mesh.
             *
             */
            std::vector<Triangle*> getTriangleList();

            /**
             * @brief Gets a set of all Vertices in mesh.
             *
             */
            std::vector<Vertex*> extractVerticesList();

            std::string asJsonText() override;

            void updateMBB() override;
        };
    }
}


#endif //TM2IN_TRIANGLEMESH_H
