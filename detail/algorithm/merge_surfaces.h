//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_DETAIL_MERGE_SURFACES_H
#define TM2IN_DETAIL_MERGE_SURFACES_H

#include "features/IndoorComponent.h"

using namespace std;
using namespace TM2IN::Wall;

namespace TM2IN{
    namespace detail{
        namespace algorithm{
            /**
             * @ingroup imp_details
             */
            class SurfaceMerger{
            public:
                SurfaceMerger(double t1, double t2);
                bool mergeSurfaces(vector<Wall::TriangulatedSurface *> surfaceList, vector<Wall::TriangulatedSurface*>& result);
            private:
                double thres1;
                double thres2;

                int merge(Wall::TriangulatedSurface  *origin, Wall::TriangulatedSurface  *piece);
                bool check_merge_condition(Vector_3 &big, Vector_3 &small);
                bool is_coplanar(Vector_3 &big, Vector_3 &small);
            };
            /**
             * @ingroup imp_details
             */
            bool merging_invalid_test(vector<HalfEdge *> new_edges, Vector_3 newNormal);
        }
    }
}

#endif //TM2IN_DETAIL_MERGE_SURFACES_H
