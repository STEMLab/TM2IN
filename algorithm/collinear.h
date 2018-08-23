//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_ALGORITHM_COLLINEAR_H
#define TM2IN_ALGORITHM_COLLINEAR_H

#include <features/Vertex.h>

namespace TM2IN{
    /**
     * Threshold value for determining collinear vectors.
     * @warning Default value is 0.001.
     */
    extern double threshold_collinear;
    namespace algorithm{
        /**
         *  @ingroup public_api
         * @brief Compares angle between Vector(origin, v1) and Vector(origin, v2) and the specific angle.
         */
        bool has_lower_angle_than(Vertex* origin, Vertex* v1, Vertex* v2, double angle);
        /**
         *  @ingroup public_api
         * @brief Determines whether the line string (start_p, check_p, end_p) is collinear.
         *
         */
        bool isCollinear(Vertex *start_p, Vertex *check_p, Vertex *end_p);
    }
}

#endif //TM2IN_COLLINEAR_H
