#include "data/SurfaceGraph.h"

void SurfaceGraph::makeAdjacentGraph(vector<Surface*>& surface_list){
    for (ull i = 0 ; i < surface_list.size() ; i++){
        Surface* surface_i = surface_list[i];
        for (ull j = 0 ; j < surface_list.size() ; j ++){
            ll s_i, s_j;
            if (surface_i->isAdjacent(surface_list[j], s_i, s_j)){
                adjList[i].push_back(make_pair(j, make_pair(s_i,s_j)));
            }
        }
    }
}
