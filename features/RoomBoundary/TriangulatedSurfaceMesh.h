//
// Created by dongmin on 18. 7. 30.
//

#ifndef TM2IN_TRIANGULATEDSURFACEMESH_H
#define TM2IN_TRIANGULATEDSURFACEMESH_H

#include <features/IndoorComponent.h>
#include "util.h"

using namespace TM2IN::Wall;

namespace TM2IN {
    namespace RoomBoundary {
        /**
         * @ingroup geo_features
         * Mesh consists of only TM2IN::TriangulatedSurface.
         */
        typedef TM2IN::RoomBoundary::TriangulatedSurfaceMesh TSM;
        class TriangulatedSurfaceMesh : public IndoorComponent{
        public:
            /**
             * @brief Constructor with a TriangleMesh
             *
             */
            TriangulatedSurfaceMesh(TriangleMesh* tm);

            /**
             * @brief Gets a list of all Surface.
             *
             */
            std::vector<TriangulatedSurface *> surface_list();

            /**
             * @brief Sets a list of Surface to patches.
             *
             */
            void setSurfacesList(std::vector<TriangulatedSurface *> vector);

            /**
             * @brief Surface
             *  @todo encapsulate
             */
            std::vector<TriangulatedSurface *> surfaces;

            /**
             * @brief Validate with easy restriction
             */
            int surface_easy_validation();

            /**
             * @brief Validate with strict restriction
             */
            int surface_strict_validation();

            /**
             * @brief Updates normal vectors of all patches
             */
            int update_surfaces_normal();

            /**
             * @brief Gets the number of patches in the mesh.
             */
            ull num_of_surfaces();

            /**
             * @brief Checks this is closed.
             */
            bool isClosed();

            std::string asJsonText() override;

            void updateMBB() override;
        };
    }
}


#endif //TM2IN_TRIANGULATEDSURFACEMESH_H
