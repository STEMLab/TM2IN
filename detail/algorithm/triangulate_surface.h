//
// Created by dongmin on 18. 7. 20.
//

#ifndef TM2IN_TRIANGULATE_SURFACE_H
#define TM2IN_TRIANGULATE_SURFACE_H

#include "features/Surface.h"
#include "features/Triangle.h"
#include "detail/cgal_config.h"
#include "detail/cgal/plane.h"
#include "detail/cgal/polygon.h"

namespace TM2IN {
    namespace detail {
        namespace algorithm {
            /**
             * @ingroup imp_details
             */
            int triangulate_surface(Surface* sf, vector<Triangle*>& result);
        }
    }
}

#endif //TM2IN_TRIANGULATE_SURFACE_H
