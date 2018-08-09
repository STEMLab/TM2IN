//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_GEOMETRY_H
#define TM2IN_GEOMETRY_H

#include "features/Surface.h"

namespace TM2IN {
    namespace detail {
        namespace cgal {
            /**
             * @ingroup imp_details
             * @brief Chekcs Bbox_3 intersects
             */
            bool has_bbox_intersect(IndoorComponent *s1, IndoorComponent *s2);
        }
    }
}

#endif //TM2IN_GEOMETRY_H
