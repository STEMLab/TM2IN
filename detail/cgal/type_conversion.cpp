//
// Created by dongmin on 18. 7. 19.
//

#include "type_conversion.h"

#include "features/HalfEdge.h"
#include "features/Vertex.h"
#include "features/Wall/Triangle.h"

namespace TM2IN {
    namespace detail {
        namespace cgal {
            Kernel::Point_3 to_CGAL_Point_3(Vertex &v) {
                return Point_3(v.x(),v.y(),v.z());;
            }

            Kernel::Triangle_3 to_CGAL_Triangle_3(Wall::Triangle &tri) {
                Point_3 p1 = tri.vertex(0)->CGAL_point();
                Point_3 p2 = tri.vertex(1)->CGAL_point();
                Point_3 p3 = tri.vertex(2)->CGAL_point();

                Triangle_3 cgal_triangle(p1,p2,p3);
                return cgal_triangle;
            }

            Segment_3 to_CGAL_Segment_3(HalfEdge *he) {
                Point_3 p1 = he->vertices[0]->CGAL_point();
                Point_3 p2 = he->vertices[1]->CGAL_point();
                Segment_3 seg(p1, p2);
                return  seg;
            }

            Bbox_3 to_CGAL_bbox3(MinimumBoundingBox &box) {
                return Bbox_3(box.min(0), box.min(1), box.min(2), box.max(0), box.max(1), box.max(2));
            }
        }
    }
}