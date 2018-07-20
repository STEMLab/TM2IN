//
// Created by dongmin on 18. 7. 19.
//

#include "surface_neighbor.h"


namespace TM2IN{
    namespace detail{
        namespace algorithm{
            bool isNeighbor(Surface *origin, Surface *piece) {
                for (HalfEdge* he : origin->getBoundaryEdgesList()){
                    if (he->oppositeEdge != NULL && he->oppositeEdge->parent == piece) return true;
                }
                return false;
            }

            bool findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j){
                ll piece_size = vi.size();
                ll origin_size = vj.size();

                for (ll i = 0 ; i < piece_size ;i++){
                    for (ll j = origin_size - 1 ; j >= 0 ; j--){
                        if (vi[i] == vj[j]){
                            ll next_i = i + 1 == piece_size? 0 : i+1;
                            ll next_j = j-1 == -1? origin_size-1 : j-1;
                            if (vi[next_i] == vj[next_j]){
                                middle_i = i;
                                middle_j = j;
                                return true;
                            }
                        }
                    }
                }

                return false;
            }

            int findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j){
                ll piece_size = (ll)vi.size();
                ll origin_size = (ll)vj.size();

                ll i = middle_i, j = middle_j;

                ll next_i = i + 1 == piece_size? 0 : i+1;
                ll next_j = j-1 == -1? origin_size-1 : j-1;

                while (vi[next_i] == vj[next_j])
                {
                    if (next_i == middle_i || next_j == middle_j){
                        return 1;
                    }

                    i = next_i;
                    j = next_j;

                    next_i = i + 1 == piece_size? 0 : i + 1;
                    next_j = j - 1 == -1? origin_size-1 : j - 1;
                }
                end_i = i;
                start_j = j;

                i = middle_i;
                j = middle_j;

                next_i = i - 1 == -1? piece_size -1 : i - 1;
                next_j = j + 1 == origin_size? 0 : j + 1;

                while (vi[next_i] == vj[next_j])
                {
                    if (next_i == middle_i || next_j == middle_j){
                        return 1;
                    }

                    i = next_i;
                    j = next_j;

                    next_i = i - 1 == -1? piece_size -1 : i - 1;
                    next_j = j + 1 == origin_size? 0 : j + 1;
                }
                start_i = i;
                end_j = j;
                return 0;
            }

            int constructNeighborInfo(Surface * piece, Surface * origin, neighbor_info & ni) {
                vector<Vertex*> origin_vertex_list = origin->getVerticesList();
                vector<Vertex*> piece_vertex_list = piece->getVerticesList();

                ll piece_middle = -1, origin_middle = -1;
                if (!findShareVertex(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle)) return 1;

                ll lastVertex_piece = -1, lastVertex_origin = -1;
                ll firstVertex_piece = -1, firstVertex_origin = -1;

                int result = findStartAndEnd(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle, firstVertex_piece, lastVertex_piece, firstVertex_origin, lastVertex_origin);
                if (result) return 1;

                ni.firstVertex_origin = firstVertex_origin;
                ni.firstVertex_piece = firstVertex_piece;
                ni.lastVertex_origin = lastVertex_origin;
                ni.lastVertex_piece = lastVertex_piece;

                return 0;
            }


        }
    }
}
