//
// Created by dongmin on 18. 7. 24.
//

#ifndef TM2IN_COMPARE_H
#define TM2IN_COMPARE_H

#include "detail/cgal_config.h"
#include "features/Vertex.h"

namespace TM2IN {
    namespace algorithm {
        /**
         * 오름차순
         * @param a
         * @param b
         * @return
         */
        bool greater(Vertex * a, Vertex * b);
    }
}

#endif //TM2IN_COMPARE_H
