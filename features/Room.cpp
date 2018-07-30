//
// Created by dongmin on 18. 7. 25.
//

#include "features/RoomBoundary/TriangleMesh.h"

#include "Room.h"

namespace TM2IN{
    Room::Room(RoomBoundary::TriangleMesh *tm) {
        this->tm_boundary = tm;
        this->type=IND_TYPE ::Room;
    }

    RoomBoundary::TriangleMesh *Room::getTm_boundary() const {
        return tm_boundary;
    }

    void Room::setTm_boundary(RoomBoundary::TriangleMesh *tm_boundary) {
        Room::tm_boundary = tm_boundary;
    }

    RoomBoundary::TriangulatedSurfaceMesh *Room::getTsm_boundary() const {
        return tsm_boundary;
    }

    void Room::setTsm_boundary(RoomBoundary::TriangulatedSurfaceMesh *tsm_boundary) {
        Room::tsm_boundary = tsm_boundary;
    }

    RoomBoundary::PolygonMesh *Room::getPm_boundary() const {
        return pm_boundary;
    }

    void Room::setPm_boundary(RoomBoundary::PolygonMesh *pm_boundary) {
        Room::pm_boundary = pm_boundary;
    }


}