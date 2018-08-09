//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_PCA_H
#define TM2IN_PCA_H

#include <vector>

#include "features/Vertex.h"
#include "detail/cgal_config.h"

namespace TM2IN {
    namespace detail {
        namespace cgal {
            /**
             * @ingroup imp_details
             * @brief Creates PCA plane with Vertex and normal vector
             */
            Plane_3 make_PCA_plane(std::vector<Vertex *> vertices, Vector_3 normal);
            /**
             * @ingroup imp_details
             */
            Plane_3 make_simple_plane(Vector_3 pNormal);
            /**
             * @ingroup imp_details
             * @brief Projects Vertex list to plane and returns a vector of Kernel::Point_2
             */
            std::vector<Point_2> project_to_plane(std::vector<Vertex*> vertexList, Plane_3 plane);
        }
    }
}

#endif //TM2IN_PCA_H
