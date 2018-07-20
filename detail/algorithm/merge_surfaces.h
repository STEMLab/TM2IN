//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_DETAIL_MERGE_SURFACES_H
#define TM2IN_DETAIL_MERGE_SURFACES_H

#include "features/Surface.h"
#include <vector>

using namespace std;

namespace TM2IN{
    namespace detail{
        namespace algorithm{
            class SurfaceMerger{
            public:
                SurfaceMerger(double t1, double t2);
                bool mergeSurfaces(vector<Surface *> surfaceList, vector<Surface*>& result);
            private:
                double thres1;
                double thres2;

                int merge(Surface *origin, Surface *piece);
                bool check_merge_condition(Vector_3 &big, Vector_3 &small);
                bool is_coplanar(Vector_3 &big, Vector_3 &small);
            };

            bool merging_invalid_test(vector<HalfEdge *> new_edges, Vector_3 newNormal);
        }
    }
}

#endif //TM2IN_DETAIL_MERGE_SURFACES_H
