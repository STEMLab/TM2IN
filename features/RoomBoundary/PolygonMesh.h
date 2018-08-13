//
// Created by dongmin on 18. 7. 30.
//

#ifndef TM2IN_POLYGONMESH_H
#define TM2IN_POLYGONMESH_H

#include <features/IndoorComponent.h>

using namespace std;
using namespace TM2IN::Wall;

namespace TM2IN {
    namespace RoomBoundary {
        /**
         * @ingroup geo_features
         * @brief Mesh consists of only TM2IN::Polygon.
         */
        class PolygonMesh : public IndoorComponent {
        public:
            /**
             * @todo encapsulation
             */
            vector<Wall::Polygon*> polygons;

            /**
             * @brief Constructor with a Surface vector
             */
            PolygonMesh(vector<Wall::Polygon*> _polygons) : polygons(_polygons) {
                this->type=IND_TYPE ::RoomBoundary;
            };

            /**
             * @brief Tags ids to polygons
             */
            void tag_ID(string name);

            /**
             * @brief Convert to TMI2IN::TriangleMesh.
             * @warning It process triangulation automatically.
             */
            TriangleMesh *to_triangle_mesh();

            std::string asJsonText() override;

            void updateMBB() override;
        private:
        };
    }
}


#endif //TM2IN_POLYGONMESH_H
