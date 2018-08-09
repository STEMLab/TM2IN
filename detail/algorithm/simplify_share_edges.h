//
// Created by dongmin on 18. 7. 20.
//

#ifndef TM2IN_SIMPLIFY_SHARE_EDGES_H
#define TM2IN_SIMPLIFY_SHARE_EDGES_H

#include "features/Surface.h"
#include <vector>

using namespace std;

namespace TM2IN{
    namespace detail{
        namespace algorithm{
            /**
             * @ingroup imp_details
             */
            int simplify_share_edges(Surface* sf1, Surface* sf2);
        }
    }
}
#endif //TM2IN_SIMPLIFY_SHARE_EDGES_H
