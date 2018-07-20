//
// Created by dongmin on 18. 4. 10.
//

#include "Features_to_CGAL_object.h"

Triangle_3 CGAL_User::getCGALTriangle(Triangle *&pTriangle) {
    Point_3 p1 = pTriangle->vertex(0)->CGAL_point();
    Point_3 p2 = pTriangle->vertex(1)->CGAL_point();
    Point_3 p3 = pTriangle->vertex(2)->CGAL_point();

    Triangle_3 cgal_triangle(p1,p2,p3);
    return cgal_triangle;
}


