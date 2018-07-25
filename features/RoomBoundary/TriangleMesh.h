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
            std::map<Triangle*, std::vector<Triangle*>> adjMap;
        public:
            TriangleMesh(std::vector<Triangle *> _triangles);
            void makeGraph();

            bool is_closed_triangle_mesh();
        };
    }
}


#endif //TM2IN_TRIANGLEMESH_H
