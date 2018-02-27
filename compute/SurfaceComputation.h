//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H

#include "features/Surface.h"
#include "cgal/Types.h"

class SurfaceComputation {
public:
    static void flatten(Surface *&sf);

    static Vertex* getCenterPoint(Surface *pSurface);

    static Plane_3 makePlane3(Surface *&pSurface);

    static int triangulate(Surface *&pSurface);

    static std::vector<Point_2> to2D(Surface *&pSurface, Plane_3 plane);

    static void removeStraight(Surface *&pSurface);

    static void removeConsecutiveDuplicationIndex(Surface *&pSurface);

    static void removeConsecutiveDuplication(Surface *&pSurface);

    static std::vector<Segment_3> makeSegment3List(Surface *&pSurface);

    static std::vector<HalfEdge*> makeHalfEdgesList(Surface *&pSurface);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
