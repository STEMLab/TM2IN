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
        namespace cgal {
            /**
             * @ingroup imp_details
             * @brief Generates CGAL::Polygon_2
             */
            Polygon_2 make_CGAL_polygon(vector<Point_2> surface2D);
            /**
             * @ingroup imp_details
             * @brief Partitions Polygon_2 to a vector of Polygon_2
             */
            vector<Polygon_2> convexPartition(Polygon_2 polygon);
        }
    }
}

#endif //TM2IN_POLYGON_H

