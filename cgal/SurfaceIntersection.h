// will be removed
//
// Created by dongmin on 18. 1. 29.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_SURFACEINTERSECTION_H
#define TRIANGLEMESHTOCLEARSOLID_SURFACEINTERSECTION_H

#include "detail/cgal_config.h"
#include "features/Surface.h"

class SurfaceIntersection {
public:
    static std::vector<Surface *> resolveSelfIntersection(Surface *&pSurface);

    static void resolveEasySelfIntersection(Surface *&pSurface);

    static int makeNewIntersectionVertex(Surface *&pSurface);

    static bool checkSelfIntersection(Surface *&pSurface);

    static bool checkSelfIntersection(Surface *&pSurface, int& a, int& b);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACEINTERSECTION_H
