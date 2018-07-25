//
// Created by dongmin on 18. 7. 23.
//

#include "geometry.h"


namespace TM2IN {
    namespace detail {
        namespace cgal {
            bool has_bbox_intersect(Surface *s1, Surface *s2){
                CGAL::Bbox_3 b1 = s1->getMBB()->CGAL_bbox3();
                CGAL::Bbox_3 b2 = s2->getMBB()->CGAL_bbox3();
                return CGAL::do_intersect(b1,b2);
            }
        }
    }
}