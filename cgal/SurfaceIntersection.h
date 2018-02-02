//
// Created by dongmin on 18. 1. 29.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_SURFACEINTERSECTION_H
#define TRIANGLEMESHTOCLEARSOLID_SURFACEINTERSECTION_H

#include "cgal/Types.h"
#include "features/Surface.h"

class SurfaceIntersection {
public:
    static std::vector<Surface *> resolveSelfIntersection(Surface *&pSurface);

    static void resolveEasySelfIntersection(Surface *&pSurface);

    static bool makeNewIntersectionVertex(Surface *&pSurface);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACEINTERSECTION_H
