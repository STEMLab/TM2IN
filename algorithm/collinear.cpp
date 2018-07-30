//
// Created by dongmin on 18. 7. 23.
//
#include "collinear.h"

#include "cgal/vector_angle.h"
#include "features/Vertex.h"
#include "detail/cgal_config.h"

namespace TM2IN {
    double threshold_collinear = 0.001;
    namespace algorithm {
        bool has_lower_angle_than(Vertex* origin, Vertex* v1, Vertex* v2, double degree){
            Vector_3 vec1(origin->CGAL_point(),v1->CGAL_point());
            Vector_3 vec2(origin->CGAL_point(),v2->CGAL_point());
            assert(vec1.squared_length() > 0 && vec2.squared_length() > 0);

            return TM2IN::cgal::has_lower_angle_than(vec1, vec2, degree);
        }
        bool isCollinear(Vertex *start_p, Vertex *check_p, Vertex *end_p){
            return (has_lower_angle_than(start_p, check_p, end_p, TM2IN::threshold_collinear)
                    || has_lower_angle_than(end_p, check_p, start_p, TM2IN::threshold_collinear)
                    || has_lower_angle_than(check_p, end_p, start_p, TM2IN::threshold_collinear));
        }
    }
}