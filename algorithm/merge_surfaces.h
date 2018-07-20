//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_MERGE_SURFACES_H
#define TM2IN_MERGE_SURFACES_H

#include "features/PolyhedralSurface.h"
#include "detail/algorithm/merge_surfaces.h"

namespace TM2IN{
    namespace algorithm{
        bool mergeSurfaces(PolyhedralSurface* ps, double thres1, double thres2);
        bool mergeSurfaces(vector<Surface*>& surfaceList, double thres1, double thres2, vector<Surface*>& newSurfaceList);
        bool mergeTriangles(vector<Triangle*>& triangleList, double thres1, double thres2, vector<Surface*>& newSurfaceList);
        int cleanMergedSurfaces(PolyhedralSurface* ps);
    }
}
#endif //TM2IN_MERGE_SURFACES_H
