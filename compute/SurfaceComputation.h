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

    static Plane_3 getPlane3WithPCA(Surface *&pSurface);
    static Plane_3 getPlane3WithCenter(Surface *&pSurface);
    static Plane_3 getPlane3WithMBB(Surface *&pSurface);
    static Plane_3 getSimplePlane3WithNormal(Vector_3 pNormal);

    static int triangulate(Surface *&pSurface);

    static std::vector<Point_2> projectTo3DPlane(Surface *&pSurface, Plane_3 plane);

    static void removeStraight(Surface *&pSurface);

    static void removeConsecutiveDuplicationIndex(Surface *&pSurface);

    static void removeConsecutiveDuplication(Surface *&pSurface);

    static std::vector<Segment_3> makeSegment3List(Surface *&pSurface);
    static std::vector<Segment_2> makeSegment2List(Surface *&pSurface, Plane_3 plane3);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
