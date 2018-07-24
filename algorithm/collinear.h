//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_ALGORITHM_COLLINEAR_H
#define TM2IN_ALGORITHM_COLLINEAR_H

#include <features/Vertex.h>

namespace TM2IN{
    extern double threshold_collinear;
    namespace algorithm{
        bool has_lower_angle_than(Vertex* origin, Vertex* v1, Vertex* v2, double angle);
        bool isCollinear(Vertex *start_p, Vertex *check_p, Vertex *end_p);
    }
}

#endif //TM2IN_COLLINEAR_H
