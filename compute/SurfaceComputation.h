//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H

#include "features/Surface.h"
#include "detail/cgal_config.h"

class SurfaceComputation {
public:
    static Vertex* getCenterPoint(Surface *pSurface);

    static Plane_3 getPlane3WithCenter(Surface *&pSurface);
    static Plane_3 getPlane3WithMBB(Surface *&pSurface);

    static int triangulate(Surface *&pSurface, std::vector<Triangle*>& result);

    static void removeStraight(Surface *&pSurface);

    static void removeConsecutiveDuplicationIndex(Surface *&pSurface);

    static std::vector<Segment_3> makeSegment3List(Surface *&pSurface);
    static std::vector<Segment_2> makeSegment2List(Surface *&pSurface, Plane_3 plane3);
};

namespace TMIC{
    double computeError(Surface *&pSurface);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
