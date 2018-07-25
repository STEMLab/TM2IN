//
// Created by dongmin on 18. 7. 20.
//

#ifndef TM2IN_TRIANGULATION_H
#define TM2IN_TRIANGULATION_H

#include "features/Surface.h"
#include "features/Triangle.h"
#include "detail/cgal_config.h"
#include "detail/cgal/plane.h"
#include "detail/cgal/polygon.h"

namespace TM2IN {
    namespace algorithm {
        int triangulate(TM2IN::Surface *pSurface, std::vector<TM2IN::Triangle*>& result);
    }
}

#endif //TM2IN_TRIANGULATION_H
