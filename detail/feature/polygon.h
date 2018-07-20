//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_POLYGON_H
#define TM2IN_POLYGON_H

#include "detail/cgal_config.h"
#include <vector>

using namespace std;

namespace TM2IN {
    namespace detail {
        namespace ext_feature {
            Polygon_2 make_CGAL_polygon(vector<Point_2> surface2D);
        }
    }
}

#endif //TM2IN_POLYGON_H

