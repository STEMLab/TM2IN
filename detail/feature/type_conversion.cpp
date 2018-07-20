//
// Created by dongmin on 18. 7. 19.
//

#include "type_conversion.h"

namespace TM2IN {
    namespace detail {
        namespace ext_feature {
            Kernel::Point_3 to_CGAL_Point_3(Vertex &v) {
                return Point_3(v.x(),v.y(),v.z());;
            }
        }
    }
}