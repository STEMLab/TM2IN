//
// Created by dongmin on 18. 4. 10.
//

#include "Features_to_CGAL_object.h"

Triangle_3 TMIC::getCGALTriangle(Triangle *&pTriangle) {
    Point_3 p1 = TMIC::getCGALPoint(pTriangle->vertex(0));
    Point_3 p2 = TMIC::getCGALPoint(pTriangle->vertex(1));
    Point_3 p3 = TMIC::getCGALPoint(pTriangle->vertex(2));

    Triangle_3 cgal_triangle(p1,p2,p3);
    return cgal_triangle;
}

Point_3 TMIC::getCGALPoint(Vertex *v) {
    return Point_3(v->x(),v->y(),v->z());;
}

