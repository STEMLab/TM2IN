//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_CGAL_VECTOR_H
#define TM2IN_CGAL_VECTOR_H

#include "detail/cgal_config.h"

namespace TM2IN{
    namespace cgal{
        /**
         * @brief Returns the angle between v1 and v2.
         * @ingroup public_api
         */
        double getAngle(Vector_3& v1, Vector_3& v2);
        /**
         * @brief Checks the angle between v1 and v2 is lower than the angle
         * @ingroup public_api
         */
        bool has_lower_angle_than(Vector_3 &nv1, Vector_3 &nv2, double angle);
    }
}



#endif //TM2IN_CGAL_VECTOR_H
