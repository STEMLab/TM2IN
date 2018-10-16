//
// Created by dongmin on 18. 7. 25.
//

#ifndef TM2IN_ROOM_H
#define TM2IN_ROOM_H

#include "features/IndoorComponent.h"

namespace TM2IN {
    /**
     * @ingroup geo_features
     */
    class Room : public IndoorComponent{
    private:
        RoomBoundary::TriangleMesh* tm_boundary;
        RoomBoundary::TriangulatedSurfaceMesh* tsm_boundary;
        RoomBoundary::PolygonMesh* pm_boundary = NULL;

    public:
        /**
         * @brief Constructor with TriangleMesh
         *
         */
        Room(RoomBoundary::TriangleMesh* tm);

        /**
         * @brief Returns TriangleMesh boundary
         */
        RoomBoundary::TriangleMesh *getTm_boundary() const;
        void setTm_boundary(RoomBoundary::TriangleMesh *tm_boundary);

        /**
         * @brief Returns TriangulatedSurfaceMesh boundary
         */
        RoomBoundary::TriangulatedSurfaceMesh *getTsm_boundary() const;
        void setTsm_boundary(RoomBoundary::TriangulatedSurfaceMesh *tsm_boundary);

        /**
         * @brief Returns PolygonMesh boundary
         */
        RoomBoundary::PolygonMesh *getPm_boundary() const;
        void setPm_boundary(RoomBoundary::PolygonMesh *pm_boundary);

    };
}


#endif //TM2IN_ROOM_H
