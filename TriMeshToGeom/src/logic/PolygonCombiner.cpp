#include "logic/PolygonCombiner.h"

bool PolygonCombiner::combine(CombinedPolygon* origin, CombinedPolygon* piece, Checker* checker){
     // check Polygon is in near polygon or not
    if (!isNeighbor(origin, piece)) return false;
    if (!checker->isSamePlanar(origin->av_normal, piece->av_normal)) return false;
    //if (CGALCalculation::getAngle(origin->av_normal, piece->av_normal) > 45.0) return false;

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;


    ll origin_size = origin->getLength();
    ll piece_size = piece->getLength();
    for (ll i = 0 ; i < piece_size ;i++){
        bool escape = false;
        for (ll j = origin_size - 1 ; j >= 0 ; j--){
            if (piece->v_list[i] == origin->v_list[j]){
                start_i = i;
                start_j = j;
                while (piece->v_list[i] == origin->v_list[j]){
                    i++; j--;
                    if (i == piece_size) i = 0;
                    if (j == -1) j = origin_size - 1;
                }
                end_i = i;
                end_j = j;
                escape = true;
                break;
            }
        }
        if (escape) break;
    }

    if (start_i == -1) return false;
    if (start_j - end_j == 1 || end_j - start_j == origin->getLength()-1) {
        //cout << "One Vertex Overlap" << endl;
        return false;
    }
    for (ll i = end_i ; i != start_i ; i++){
        if (i == piece_size){
            i = -1;
            continue;
        }
        for (ll j = end_j ; j != start_j ; j--){
            if (j == -1){
                j = origin_size;
                continue;
            }

            if (piece->v_list[i] == origin->v_list[j]) return false;
        }
    }


    vector<Vertex*> new_v_list;

    for (ll j = start_j; ; ){
        new_v_list.push_back(origin->v_list[j]);
        j++;
        if (j == origin_size) j = 0;
        if (j == end_j) break;
    }
    for (ll i = end_i ; ;){
        new_v_list.push_back(piece->v_list[i]);
        i++;
        if (i == piece_size) i = 0;
        if (i == start_i) break;
    }

    origin->v_list.clear();
    origin->v_list = new_v_list;
    origin->av_normal = origin->av_normal + piece->av_normal;
    origin->sq_area += piece->sq_area;
    origin->setMBB(piece);

    return true;

}


bool PolygonCombiner::isNeighbor(CombinedPolygon* cp1, CombinedPolygon* cp2){
    //TODO
    return true;
}
