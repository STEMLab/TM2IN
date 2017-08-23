#include "logic/CleanPolygonMaker.h"

bool CleanPolygonMaker::combine(Surface* origin, Surface* piece, Checker* checker){
     // check Polygon is in near polygon or not
    if (!isNeighbor(origin, piece)) return false;

    //if (CGALCalculation::getAngle(origin->av_normal, piece->av_normal) > 45.0) return false;

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    ll middle_i = -1, middle_j = -1;

    ll origin_size = origin->getLength();
    ll piece_size = piece->getLength();

    if (!findShareVertex(piece->v_list, origin->v_list, middle_i, middle_j)) return false;

    /**< [start_i, end_i] */
    findStartAndEnd(piece->v_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j);


    int seg_num = piece->getSegmentsNumber(end_i, start_i);
    if (seg_num == -1)
    {
        exit(-1);
    }
    else if (seg_num == 0){
        /**< Only One Vertex Same*/
        return false;
    }
    else if (seg_num == 1){
    }
    else{
        if (!checker->isSamePlanar(origin->av_normal, piece->av_normal)) return false;
    }


    //TODO : after sorting and check duplicate..
    for (ll i = end_i + 1 ; i != start_i ; i++){
        if (i == piece_size){
            i = -1;
            continue;
        }
        for (ll j = end_j - 1 ; j != start_j ; j--){
            if (j == -1){
                j = origin_size;
                continue;
            }

            if (piece->v_list[i] == origin->v_list[j]) {
                return false;
            }
        }
    }


    vector<Vertex*> new_v_list;

    for (ll j = start_j; ; ){
        new_v_list.push_back(origin->v_list[j]);
        j++;
        if (j == origin_size) j = 0;
        if (j == end_j) break;
    }
    for (ll i = end_i; ;){
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

    if (origin ->checkDuplicate(checker)){
        cout << "Duplicate" << endl;
        exit(-1);
    }
    return true;
}

bool CleanPolygonMaker::findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j){
    ll piece_size = vi.size();
    ll origin_size = vj.size();

    for (ll i = 0 ; i < piece_size ;i++){
        for (ll j = origin_size - 1 ; j >= 0 ; j--){
            if (vi[i] == vj[j]){
                middle_i = i;
                middle_j = j;
                return true;
            }
        }
    }

    return false;
}
void CleanPolygonMaker::findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j){
    ll piece_size = vi.size();
    ll origin_size = vj.size();

    ll i = middle_i, j = middle_j;

    ll next_i = i + 1 == piece_size? 0 : i+1;
    ll next_j = j-1 == -1? origin_size-1 : j-1;
    while (vi[next_i] == vj[next_j])
    {
        i = next_i;
        j = next_j;

        next_i = i + 1 == piece_size? 0 : i+1;
        next_j = j-1 == -1? origin_size-1 : j-1;
    }
    end_i = i;
    end_j = j;

    i = middle_i;
    j = middle_j;

    next_i = i - 1 == -1? vi.size() -1 : i - 1;
    next_j = j + 1 == origin_size? 0 : j + 1;
    while (vi[next_i] == vj[next_j])
    {
        i = next_i;
        j = next_j;

        next_i = i - 1 == -1? vi.size() -1 : i - 1;
        next_j = j + 1 == origin_size? 0 : j + 1;
    }
    start_i = i;
    start_j = j;
}

bool CleanPolygonMaker::isNeighbor(Surface* cp1, Surface* cp2){
    //TODO
    return true;
}


int CleanPolygonMaker::simplifyShareLine(Surface* origin, Surface* piece){
    if (!isNeighbor(origin, piece)) return false;

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    ll middle_i = -1, middle_j = -1;

    ll origin_size = origin->getLength();
    ll piece_size = piece->getLength();


    for (ll i = 0 ; i < piece_size ;i++){
        bool escape = false;
        for (ll j = origin_size - 1 ; j >= 0 ; j--){
            if (piece->v_list[i] == origin->v_list[j]){
                middle_i = i;
                middle_j = j;
                escape = true;
                break;
            }
        }
        if (escape) break;
    }

    if (middle_i == -1) return false;


    /**< [start_i, end_i] */
    findStartAndEnd(piece->v_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j);


    return 0;
}

