//
// Created by dongmin on 18. 7. 25.
//

#ifndef TM2IN_ROOM_H
#define TM2IN_ROOM_H

#include "features/IndoorComponent.h"

namespace TM2IN {
    class Room : public IndoorComponent{
    private:
        RoomBoundary::TriangleMesh* tm_boundary;
        RoomBoundary::TriangulatedSurfaceMesh* tsm_boundary;
        RoomBoundary::PolygonMesh* pm_boundary;
    public:
        Room(RoomBoundary::TriangleMesh* tm);

    };
}


#endif //TM2IN_ROOM_H
