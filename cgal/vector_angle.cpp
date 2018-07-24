//
// Created by dongmin on 18. 7. 23.
//
#define PI 3.14159265

#include "detail/cgal/vector_3.h"
#include "vector_angle.h"



namespace TM2IN {
    namespace cgal {
        Vector_3 simple_normal_vector_list6[6] = {
                Vector_3(1,0,0), //0, 3
                Vector_3(0,1,0),
                Vector_3(0,0,1),
                Vector_3(-1,0,0),
                Vector_3(0,-1,0),
                Vector_3(0,0,-1)
        };

        double getAngle(Vector_3 &nv1, Vector_3 &nv2) {

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


        int findNormalType6(Vector_3& nv)
        {
            int type = 0;
            double diff = 90.0;
            for (int i = 0 ; i < 6 ; i++){
                double temp_diff = getAngle(simple_normal_vector_list6[i], nv);
                if (temp_diff < diff){
                    diff = temp_diff;
                    type = i;
                }
            }
            return type;
        }
    }
}