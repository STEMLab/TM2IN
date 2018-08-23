//
// Created by dongmin on 18. 7. 20.
//

#ifndef TM2IN_TRIANGULATION_H
#define TM2IN_TRIANGULATION_H

#include "features/IndoorComponent.h"
#include "detail/cgal_config.h"
#include "detail/cgal/plane.h"
#include "detail/cgal/polygon.h"

namespace TM2IN {
    namespace algorithm {
        /**
         * @brief Triangulate one TM2IN::Surface to multiple TM2IN::Triangle objects.
         * @ingroup public_api
         */
        int triangulate(TM2IN::Wall::Surface *pSurface, std::vector<TM2IN::Wall::Triangle*>& result);
    }
}

#endif //TM2IN_TRIANGULATION_H
