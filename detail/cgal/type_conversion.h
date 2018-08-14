//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_TYPE_CONVERSION_H
#define TM2IN_TYPE_CONVERSION_H

#include "features/IndoorComponent.h"
#include "detail/cgal_config.h"

namespace TM2IN{
    namespace detail{
        namespace cgal{
            /**
             * @ingroup imp_details
             * @brief Vertex to CGAL Point_3
             */
            Kernel::Point_3 to_CGAL_Point_3(Vertex& vt);
            /**
             * @ingroup imp_details
             * @brief Wall::Triangle to CGAL Triangle_3
             */
            Kernel::Triangle_3 to_CGAL_Triangle_3(Wall::Triangle& tri);
            /**
             * @ingroup imp_details
             * @brief HalfEdge to CGAL Segment_3
             */
            Segment_3 to_CGAL_Segment_3(HalfEdge* he);
            /**
             * @ingroup imp_details
             * @brief MinimumBoundingBox to CGAL Bbox_3
             */
            Bbox_3 to_CGAL_bbox3(MinimumBoundingBox& box);
        }
    }
}

#endif //TM2IN_TYPE_CONVERSION_H
