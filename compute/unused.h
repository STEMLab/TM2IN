#ifndef TM2IN_UNUSED_H
#define TM2IN_UNUSED_H

#include "features/Surface.h"
#include "detail/cgal_config.h"

namespace TM2IN {
    namespace unused {
        /**
         * @brief Removes straight vertices in Surface
         * @ingroup unused
         */
        void removeStraight(Surface*& pSurface);
        /**
         * @brief Converts Kernel::Segment_2 list from Surface
         * @ingroup unused
         */
        std::vector<Segment_2> makeSegment2List(Surface *&pSurface, Plane_3 plane3);
        /**
         * @brief Resolves self-intersection in Surface
         * @ingroup unused
         */
        std::vector<Surface *> resolveSelfIntersection(Surface *&pSurface);
        /**
         * @brief Resolves self-intersection in Surface
         * @ingroup unused
         */
        void resolveEasySelfIntersection(Surface *&pSurface);
        /**
         * @brief Resolves self-intersection in Surface
         * @ingroup unused
         */
        int makeNewIntersectionVertex(Surface *&pSurface);
    }
}

#endif //TM2IN_UNUSED_H
