//
// Created by dongmin on 18. 7. 23.
//

#include <cgal/vector_angle.h>
#include "vector_3.h"

namespace TM2IN {
    namespace detail {
        namespace cgal {
            double getCosineValue(Vector_3 &nv1, Vector_3 &nv2) {
                assert(nv1.squared_length() > 0 && nv2.squared_length() > 0);
                double nv1_length = sqrt(nv1.squared_length());
                double nv2_length = sqrt(nv2.squared_length());

                Vector_3 unit_nv1 = nv1 / nv1_length;
                Vector_3 unit_nv2 = nv2 / nv2_length;

                double inner = (unit_nv1 * unit_nv2);
                double cos = inner;
                if (cos > 0.99999) cos = 1;
                else if (cos < -0.99999) cos = -1;
                return cos;
            }

            int find_closest_vector(Vector_3 &nv, std::vector<Vector_3> &vector_list) {
                int type = 0;
                double diff = 90.0;
                for (int i = 0 ; i < vector_list.size() ; i++){
                    double temp_diff = TM2IN::cgal::getAngle(vector_list[i], nv);
                    if (temp_diff < diff){
                        diff = temp_diff;
                        type = i;
                    }
                }
                return type;
            }
        }
    }
}