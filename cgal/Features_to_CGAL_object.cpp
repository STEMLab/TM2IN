//
// Created by dongmin on 18. 4. 10.
//

#include "Features_to_CGAL_object.h"

Triangle_3 CGAL_User::getCGALTriangle(Triangle *&pTriangle) {
    Point_3 p1 = CGAL_User::getCGALPoint(pTriangle->vertex(0));
    Point_3 p2 = CGAL_User::getCGALPoint(pTriangle->vertex(1));
    Point_3 p3 = CGAL_User::getCGALPoint(pTriangle->vertex(2));

    Triangle_3 cgal_triangle(p1,p2,p3);
    return cgal_triangle;
}

Point_3 CGAL_User::getCGALPoint(Vertex *v) {
}

