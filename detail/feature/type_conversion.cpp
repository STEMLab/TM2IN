//
// Created by dongmin on 18. 7. 19.
//

#include "type_conversion.h"

namespace TM2IN {
    namespace detail {
        namespace feature {
            Kernel::Point_3 to_CGAL_Point_3(Vertex &v) {
                return Point_3(v.x(),v.y(),v.z());;
            }

            Kernel::Triangle_3 to_CGAL_Triangle_3(Triangle &tri) {
                Point_3 p1 = tri.vertex(0)->CGAL_point();
                Point_3 p2 = tri.vertex(1)->CGAL_point();
                Point_3 p3 = tri.vertex(2)->CGAL_point();

                Triangle_3 cgal_triangle(p1,p2,p3);
                return cgal_triangle;
            }
        }
    }
}