//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_CGAL_VECTOR_H
#define TM2IN_CGAL_VECTOR_H

#include "detail/cgal_config.h"

namespace TM2IN{
    namespace cgal{
        extern Vector_3 simple_normal_vector_list6[6];
        double getAngle(Vector_3& v1, Vector_3& v2);
        bool has_lower_angle_than(Vector_3 &nv1, Vector_3 &nv2, double angle);
        int findNormalType6(Vector_3& nv);
    }
}



#endif //TM2IN_CGAL_VECTOR_H
