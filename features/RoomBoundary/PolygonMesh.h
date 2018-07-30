//
// Created by dongmin on 18. 7. 30.
//

#ifndef TM2IN_POLYGONMESH_H
#define TM2IN_POLYGONMESH_H


#include <features/IndoorComponent.h>

using namespace std;

namespace TM2IN {
    namespace RoomBoundary {
        class PolygonMesh : public IndoorComponent {
        public:
            vector<Surface*> polygons;

            PolygonMesh(vector<Surface*> _polygons) : polygons(_polygons) {
                this->type=IND_TYPE ::RoomBoundary;
            };

            void tag_ID(string name);

            TriangleMesh *to_triangle_mesh();

            std::string asJsonText() override;

            void updateMBB() override;
        private:
        };
    }
}


#endif //TM2IN_POLYGONMESH_H
