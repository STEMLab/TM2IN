//
// Created by dongmin on 18. 7. 23.
//

#include "geometry.h"


namespace TM2IN {
    namespace detail {
        namespace feature {
            bool has_bbox_intersect(Surface *s1, Surface *s2){
                CGAL::Bbox_3 b1(s1->getMin_coords()[0],s1->getMin_coords()[1],s1->getMin_coords()[2],
                                s1->getMax_coords()[0],s1->getMax_coords()[1],s1->getMax_coords()[2]);
                CGAL::Bbox_3 b2(s2->getMin_coords()[0],s2->getMin_coords()[1],s2->getMin_coords()[2],
                                s2->getMax_coords()[0],s2->getMax_coords()[1],s2->getMax_coords()[2]);
                return CGAL::do_intersect(b1,b2);
            }
        }
    }
}