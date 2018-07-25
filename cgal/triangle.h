//
// Created by dongmin on 18. 7. 24.
//
#ifndef TM2IN_TRIANGLE_H
#define TM2IN_TRIANGLE_H

#include "detail/cgal_config.h"
#include "features/Vertex.h"

namespace TM2IN {
    namespace cgal{
        Vector_3 triangle_normal(TM2IN::Vertex *va, TM2IN::Vertex *vb, TM2IN::Vertex *vc);
        double triangle_area(Vertex *va, Vertex *vb, Vertex *vc);
        double triangle_area(Point_3 &p1, Point_3 &p2, Point_3 &p3);
    }
}

#endif //TM2IN_TRIANGLE_H
