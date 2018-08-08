//
// Created by dongmin on 18. 7. 24.
//

#ifndef TM2IN_MBB_H
#define TM2IN_MBB_H

#include "detail/cgal_config.h"
#include "features/Surface.h"

namespace TM2IN{
    namespace algorithm{
        /**
         * @ingroup public_api
         * @brief Gets CGAL::Bbox_3 for Surface list.
         */
        CGAL::Bbox_3 getMBB(std::vector<Surface *> &surfacesList);
    }
}

#endif //TM2IN_MBB_H
