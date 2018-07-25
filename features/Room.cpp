//
// Created by dongmin on 18. 7. 25.
//

#include "features/RoomBoundary/TriangleMesh.h"

#include "Room.h"

TM2IN::Room::Room(TM2IN::TriangleMesh *tm) {
    this->tm_boundary = tm;
}
