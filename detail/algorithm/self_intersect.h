//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_SELF_INTERSECT_H
#define TM2IN_SELF_INTERSECT_H
#include <vector>

#include "features/HalfEdge.h"

using namespace std;

namespace TM2IN {
    namespace detail {
        namespace algorithm {
            /**
             * @ingroup imp_details
             */
            bool has_self_intersection(vector<HalfEdge*> edges);
        }
    }
}

#endif //TM2IN_SELF_INTERSECT_H
