//
// Created by dongmin on 18. 7. 24.
//

#include "triangle.h"


namespace TM2IN {
    namespace cgal {
        double triangle_area(Vertex *va, Vertex *vb, Vertex *vc){
            Point_3 p3a(va->x(),va->y(),va->z());
            Point_3 p3b(vb->x(),vb->y(),vb->z());
            Point_3 p3c(vc->x(),vc->y(),vc->z());
            return triangle_area(p3a, p3b, p3c);
        }

        double triangle_area(Point_3 &p1, Point_3 &p2, Point_3 &p3){
            Triangle_3 tri(p1,p2,p3);
            return tri.squared_area();
        }

        Vector_3 triangle_normal(TM2IN::Vertex *va, TM2IN::Vertex *vb, TM2IN::Vertex *vc) {
            Point_3 p3a(va->x(),va->y(),va->z());
            Point_3 p3b(vb->x(),vb->y(),vb->z());
            Point_3 p3c(vc->x(),vc->y(),vc->z());

            Vector_3 vab(p3a, p3b);
            Vector_3 vac(p3a, p3c);

            return CGAL::cross_product(vab, vac);
        }
    }
}