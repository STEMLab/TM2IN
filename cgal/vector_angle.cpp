//
// Created by dongmin on 18. 7. 23.
//
#define PI 3.14159265

#include <Options.h>
#include "detail/cgal/vector_3.h"
#include "vector_angle.h"



namespace TM2IN {
    namespace cgal {

        double getAngle(Vector_3 &nv1, Vector_3 &nv2) {
            if (!Options::getInstance()->do_validation && (nv1.squared_length() == 0 || nv2.squared_length() == 0)){
                return 0;
            }
            double cos = TM2IN::detail::cgal::getCosineValue(nv1, nv2);
            double angle = acos(cos) * 180.0 / PI;

            assert(angle == angle); //check NAN
            return angle;
        }

        bool has_lower_angle_than(Vector_3 &nv1, Vector_3 &nv2, double degree){
            double angle = getAngle(nv1, nv2);
            assert(angle >= 0);
            return (angle <= degree);
        }
    }
}