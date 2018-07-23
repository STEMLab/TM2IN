//
// Created by dongmin on 18. 7. 23.
//

#include "detail/feature/type_conversion.h"
#include "self_intersect.h"

namespace TM2IN {
    namespace detail {
        namespace algorithm {
            bool has_self_intersection(vector<HalfEdge *> edges) {
                vector<Segment_3> segmentList;
                for (int i = 0 ; i < edges.size() ; i++){
                    segmentList.push_back(TM2IN::detail::feature::to_CGAL_Segment_3(edges[i]));
                }
                for (int i = 0 ; i < segmentList.size() - 2; i++) {
                    for (int j = i + 2; j < segmentList.size(); j++) {
                        if (i == 0 && j == segmentList.size() - 1) continue;
                        if (CGAL::do_intersect(segmentList[i], segmentList[j])){
                            segmentList.clear();
                            return true;
                        }
                    }
                }
                segmentList.clear();
                return false;
            }
        }
    }
}