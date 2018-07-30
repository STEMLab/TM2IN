//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_MERGE_SURFACES_H
#define TM2IN_MERGE_SURFACES_H

#include "features/IndoorComponent.h"

using namespace std;

namespace TM2IN{
    namespace algorithm{
        bool mergeSurfaces(TM2IN::RoomBoundary::TriangulatedSurfaceMesh* tsm, double thres1, double thres2);
        bool mergeSurfaces(vector<Surface*>& surfaceList, double thres1, double thres2, vector<Surface*>& newSurfaceList);
        bool mergeTriangles(vector<Triangle*>& triangleList, double thres1, double thres2, vector<Surface*>& newSurfaceList);
        int cleanMergedSurfaces(TM2IN::RoomBoundary::TriangulatedSurfaceMesh* tsm);
    }
}
#endif //TM2IN_MERGE_SURFACES_H
