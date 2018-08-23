//
// Created by dongmin on 18. 7. 24.
//

#ifndef TM2IN_COMPARE_H
#define TM2IN_COMPARE_H

#include "detail/cgal_config.h"
#include "features/Vertex.h"

class IndoorComponent;
namespace TM2IN {
    namespace algorithm {
        /**
         * @ingroup public_api
         * @brief Compares two Vertices
         */
        bool greater(Vertex * a, Vertex * b);
        /**
         * @ingroup public_api
         * @brief Compares two Indoor Component by calculating area.
         */
        bool compareArea(IndoorComponent* i, IndoorComponent* j);
    }
}

#endif //TM2IN_COMPARE_H
