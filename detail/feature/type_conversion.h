//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_TYPE_CONVERSION_H
#define TM2IN_TYPE_CONVERSION_H

#include "features/Vertex.h"
#include "detail/cgal_config.h"

namespace TM2IN{
    namespace detail{
        namespace feature{
            Kernel::Point_3 to_CGAL_Point_3(Vertex& vt);
        }
    }
}

#endif //TM2IN_TYPE_CONVERSION_H
