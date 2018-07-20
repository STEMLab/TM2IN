//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_SURFACE_NEIGHBOR_H
#define TM2IN_SURFACE_NEIGHBOR_H

#include "features/Surface.h"
#include "features/HalfEdge.h"

using namespace std;

namespace TM2IN{
    namespace detail{
        namespace algorithm{
            struct neighbor_info{
                ll lastVertex_piece = -1;
                ll lastVertex_origin = -1;
                ll firstVertex_piece = -1;
                ll firstVertex_origin = -1;
            };

            bool isNeighbor(Surface* , Surface* );
            int constructNeighborInfo(Surface* , Surface* , neighbor_info&);

            int findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j);
            bool findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j);
        }
    }
}

#endif //TM2IN_SURFACE_NEIGHBOR_H
