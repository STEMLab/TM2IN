//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_VECTOR_3_H
#define TM2IN_VECTOR_3_H

#include "detail/cgal_config.h"

namespace TM2IN {
    namespace detail {
        namespace cgal {
            /**
             * @ingroup imp_details
             * @brief Returns cosine value
             */
            double getCosineValue(Vector_3& nv1,Vector_3& nv2);

            /**
             * @ingroup imp_details
             * @brief Return closest vector from nv among vector_list
             *
             */
            int find_closest_vector(Vector_3& nv, std::vector<Vector_3>& vector_list);
        }
    }
}

#endif //TM2IN_VECTOR_3_H
