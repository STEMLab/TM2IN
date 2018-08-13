//
// Created by dongmin on 18. 7. 20.
//

#ifndef TM2IN_TRIANGULATE_SURFACE_H
#define TM2IN_TRIANGULATE_SURFACE_H

#include "features/IndoorComponent.h"
#include "detail/cgal_config.h"
#include "detail/cgal/plane.h"
#include "detail/cgal/polygon.h"

namespace TM2IN {
    namespace detail {
        namespace algorithm {
            /**
             * @ingroup imp_details
             * @brief Triangulate Wall::Surface to a vector of Wall::Triangle. 0 is succeed. 1 is fail.
             *
             */
            int triangulate_surface(TM2IN::Wall::Surface* sf, vector<TM2IN::Wall::Triangle*>& result);
        }
    }
}

#endif //TM2IN_TRIANGULATE_SURFACE_H
