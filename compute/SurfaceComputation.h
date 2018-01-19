//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H

#include "features/Surface.hpp"
#include "cgal/Types.h"

class SurfaceComputation {
public:
    static void flatten(Surface *&sf);

    static Vertex* getCenterPoint(Surface *pSurface);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACECOMPUTATION_H
