//
// Created by dongmin on 18. 7. 20.
//

#ifndef TM2IN_TRIANGULATE_SURFACE_H
#define TM2IN_TRIANGULATE_SURFACE_H

#include "features/Surface.h"
#include "features/Triangle.h"
#include "detail/cgal_config.h"
#include "detail/feature/plane.h"
#include "detail/feature/polygon.h"

namespace TM2IN {
    namespace detail {
        namespace algorithm {
            int triangulate_surface(Surface* sf, vector<Triangle*>& result);
        }
    }
}

#endif //TM2IN_TRIANGULATE_SURFACE_H
