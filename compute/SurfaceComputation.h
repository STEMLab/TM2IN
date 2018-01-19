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

    static Plane_3 getPlane3(Surface *&pSurface);

    static void resolveIntersection(Surface *&pSurface);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
