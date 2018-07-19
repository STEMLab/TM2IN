//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_PCA_H
#define TM2IN_PCA_H

#include <vector>

#include "features/Vertex.h"
#include "detail/cgal_config.h"

namespace TM2IN {
    namespace algorithm {
        Plane_3 make_PCA_plane(std::vector<Vertex *> vertices, Vector_3 normal);
    }
}

#endif //TM2IN_PCA_H
