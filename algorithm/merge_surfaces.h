//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_MERGE_SURFACES_H
#define TM2IN_MERGE_SURFACES_H

#include "features/IndoorComponent.h"

using namespace std;

namespace TM2IN{
    namespace algorithm{
        /**
         * @ingroup public_api
         * @brief Merges TM2IN::RoomBoundary::TriangulatedSurfaceMesh to larger one.
         */
        bool mergeSurfaces(TM2IN::RoomBoundary::TriangulatedSurfaceMesh* tsm, double thres1, double thres2);
        /**
         *  @ingroup public_api
         * @brief Merges the list of Surfaces. Result will be stored in 4th parameter.
         */
        bool mergeSurfaces(vector<Surface*>& surfaceList, double thres1, double thres2, vector<Surface*>& newSurfaceList);
        /**
         * @ingroup public_api
         * @brief Merges the list of Triangles. Result will be stored in 4th parameter.
         */
        bool mergeTriangles(vector<Triangle*>& triangleList, double thres1, double thres2, vector<Surface*>& newSurfaceList);
        /**
         * @ingroup public_api
         * @brief Cleans TM2IN::RoomBoundary::TriangulatedSurfaceMesh after merging.
         */
        int cleanMergedSurfaces(TM2IN::RoomBoundary::TriangulatedSurfaceMesh* tsm);
    }
}
#endif //TM2IN_MERGE_SURFACES_H
