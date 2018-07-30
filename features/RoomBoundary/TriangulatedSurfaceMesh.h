//
// Created by dongmin on 18. 7. 30.
//

#ifndef TM2IN_TRIANGULATEDSURFACEMESH_H
#define TM2IN_TRIANGULATEDSURFACEMESH_H

#include <features/IndoorComponent.h>
#include "util.h"

namespace TM2IN {
    namespace RoomBoundary {
        typedef TM2IN::RoomBoundary::TriangulatedSurfaceMesh TSM;
        class TriangulatedSurfaceMesh : public IndoorComponent{
        public:
            TriangulatedSurfaceMesh(TriangleMesh* tm);

            std::vector<Surface *> surface_list();

            void setSurfacesList(std::vector<Surface *> vector);

            std::vector<Surface *> surfaces;

            int surface_easy_validation();
            int surface_strict_validation();

            int update_surfaces_normal();

            ull num_of_surfaces();
            bool isClosed();

            std::string asJsonText() override;

            void updateMBB() override;
        };
    }
}


#endif //TM2IN_TRIANGULATEDSURFACEMESH_H
