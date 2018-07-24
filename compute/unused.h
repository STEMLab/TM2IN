#ifndef TM2IN_UNUSED_H
#define TM2IN_UNUSED_H

#include "features/Surface.h"
#include "detail/cgal_config.h"

namespace TM2IN {
    namespace unused {
        void removeStraight(Surface*& pSurface);
        std::vector<Segment_2> makeSegment2List(Surface *&pSurface, Plane_3 plane3);


        std::vector<Surface *> resolveSelfIntersection(Surface *&pSurface);
        void resolveEasySelfIntersection(Surface *&pSurface);
        int makeNewIntersectionVertex(Surface *&pSurface);
    }
}

#endif //TM2IN_UNUSED_H
