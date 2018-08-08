//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_POLYGONIZER_H
#define TM2IN_POLYGONIZER_H

#include <vector>
#include <features/RoomBoundary/TriangulatedSurfaceMesh.h>

using namespace std;

namespace TM2IN {
    namespace algorithm {
        /**
         * @ingroup public_api
         * @brief Converter TM2IN::RoomBoundary::TriangulatedSurfaceMesh to TM2IN::RoomBoundary::PolygonMesh
         */
        class Polygonizer {
        public:
            /**
             * @brief Converts TM2IN::RoomBoundary::TriangulatedSurfaceMesh to TM2IN::RoomBoundary::PolygonMesh
             */
            virtual TM2IN::RoomBoundary::PolygonMesh* run(TM2IN::RoomBoundary::TSM* space) = 0;
        };

        /**
         * Converter TM2IN::RoomBoundary::TriangulatedSurfaceMesh to TM2IN::RoomBoundary::PolygonMesh
         */
        class TrianglePolygonizer : public Polygonizer{
        public:
            /**
             * @brief Converts TM2IN::RoomBoundary::TriangulatedSurfaceMesh to TM2IN::RoomBoundary::PolygonMesh
             */
            TM2IN::RoomBoundary::PolygonMesh* run(TM2IN::RoomBoundary::TSM* space);
        };

        /**
         * Converter TM2IN::RoomBoundary::TriangulatedSurfaceMesh to TM2IN::RoomBoundary::PolygonMesh
         */
        class DividedPolygonizer : public Polygonizer {
        public:
            /**
             * @brief Converts TM2IN::RoomBoundary::TriangulatedSurfaceMesh to TM2IN::RoomBoundary::PolygonMesh
             */
            TM2IN::RoomBoundary::PolygonMesh* run(TM2IN::RoomBoundary::TSM* space);
        };

        /**
         * Converter by using PCA
         * @todo
         */
        class PCAPolygonizer : public Polygonizer {
        public:
            /**
             * @brief Converts TM2IN::RoomBoundary::TriangulatedSurfaceMesh to TM2IN::RoomBoundary::PolygonMesh
             */
            TM2IN::RoomBoundary::PolygonMesh* run(TM2IN::RoomBoundary::TSM* space);
        private:
            void scale_up(vector<Vertex *> &vertices, double scale);
            // SFCGAL::Polygon make_sf_polygon(vector<Vertex *> vertices);
        };
    }
}


#endif //TM2IN_POLYGONIZER_H
