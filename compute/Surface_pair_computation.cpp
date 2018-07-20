#include "Surface_pair_computation.h"
#include "VertexComputation.h"
#include "HalfEdgeComputation.h"
#include "VertexListComputation.h"
#include "SurfaceComputation.h"
#include <stdlib.h>
#include <features/HalfEdge.h>
#include <cgal/PolygonComputation.h>





int TMIC::doShareEdge(Surface *&surface_i, Surface *&surface_j){
    vector<HalfEdge*> iEdgeList = surface_i->getBoundaryEdgesList();
    int share_edge_count = 0;
    for (HalfEdge* edge : iEdgeList){
        if (edge->oppositeEdge != NULL && edge->oppositeEdge->parent == surface_j){
            share_edge_count++;
        }
    }
    return share_edge_count;
}




vector<int> TMIC::listShareEdgesInPiece(Surface *origin, Surface *piece) {
    vector<int> share_edges;
    int i = 0;
    for (HalfEdge* he : piece->getBoundaryEdgesList()){
        if (he->getOppositeEdge()->parent == origin){
            share_edges.push_back(i);
        }
        i++;
    }

    bool isContinue = true;
    int start_i;
    if (share_edges.size() > 1){
        for (start_i = 0; start_i < share_edges.size() - 1; start_i++) {
            if (share_edges[start_i] + 1 != share_edges[start_i+1]){
                isContinue = false;
                break;
            }
        }
    }

    if (isContinue)
        return share_edges;
    else{
        vector<int> continued_share_edges;
        for ( ; continued_share_edges.size() < share_edges.size() ;){
            start_i++;
            if (start_i == share_edges.size()) start_i = 0;

            continued_share_edges.push_back(share_edges[start_i]);
        }

        return continued_share_edges;
    }
}

vector<int> TMIC::listShareEdgesInOrigin(Surface *origin, Surface *piece, vector<int> share_edges) {
    vector<int> origin_edges;
    for (int i = 0 ; i < share_edges.size() ; i++){
        HalfEdge* he = piece->boundary_edges(share_edges[i]);
        int origin_index = origin->indexBoundaryEdge(he);
        origin_edges.push_back(origin_index);
    }
    return origin_edges;
}




