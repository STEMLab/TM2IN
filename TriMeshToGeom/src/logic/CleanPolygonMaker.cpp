#include "logic/CleanPolygonMaker.h"
#include <stdlib.h>

bool CleanPolygonMaker::combine(Surface* origin, Surface* piece, Checker* checker, double degree){
     // check Polygon is in near polygon or not
    if (!isNeighbor(origin, piece)) return false;

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    ll middle_i = -1, middle_j = -1;

    ll origin_size = origin->getLength();
    ll piece_size = piece->getLength();

    if (!findShareVertex(piece->v_list, origin->v_list, middle_i, middle_j)) return false;

    /**< [start_i, end_i], [end_j, start_j] */
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
        if (!checker->CanbeMerged(origin->av_normal, piece->av_normal, degree)) {
            return false;
        }
    }

    //find if another line share.
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

bool CleanPolygonMaker::simplifyLineSegment(Surface* origin, Surface* piece){

    ll middle_i = -1, middle_j = -1;
    ll piece_size = piece->getLength();
    ll origin_size = origin->getLength();
    vector<Vertex*>& piece_vertex_list = piece->v_list;
    vector<Vertex*>& origin_vertex_list = origin->v_list;

    bool hasTwoShareLine = false;
    for (ll i = 0 ; i < piece_size ;i++){
        for (ll j = origin_size - 1 ; j >= 0 ; j--){
            if (piece_vertex_list[i] == origin_vertex_list[j]){
                ll next_i = i + 1 == piece_size? 0 : i+1;
                ll next_j = j-1 == -1? origin_size-1 : j-1;

                ll pre_i = i - 1 == -1? piece_size-1 : i-1;
                ll pre_j = j + 1 == origin_size? 0 : j + 1;

                if (piece_vertex_list[next_i] == origin_vertex_list[next_j]
                    && piece_vertex_list[pre_i] == origin_vertex_list[pre_j]){
                    middle_i = i;
                    middle_j = j;
                    hasTwoShareLine = true;
                    break;
                }
            }
        }
        if (hasTwoShareLine) break;
    }

    if (!hasTwoShareLine) return false;
    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    findStartAndEnd(piece_vertex_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j);

    int seg_num = piece->getSegmentsNumber(start_i, end_i);
    if (seg_num <= 1) {
        cout << "simplifyLineSegment Errrrrror" << endl;
        exit(-1);
    }

    Point_3 sp = CGALCalculation::makePoint(piece_vertex_list[start_i]);
    Point_3 ep = CGALCalculation::makePoint(piece_vertex_list[end_i]);
    Line_3 line(sp, ep);

    //Translate to make it straight
    for (ll i = start_i + 1;;)
    {
        if (i == piece_vertex_list.size()) i = 0;
        if (i == end_i) break;
        Point_3 newp = line.projection(CGALCalculation::makePoint(piece_vertex_list[i]));
        piece_vertex_list[i]->translateTo({newp.x(), newp.y(), newp.z()});

        i++;
    }

    if ( end_i > start_i ){
        piece_vertex_list.erase(piece_vertex_list.begin() + start_i + 1, piece_vertex_list.begin() + end_i);
    }
    else{
        piece_vertex_list.erase(piece_vertex_list.begin() + start_i + 1, piece_vertex_list.end());
        piece_vertex_list.erase(piece_vertex_list.begin(), piece_vertex_list.begin() + end_i);
    }

    if ( start_j > end_j ){
        origin_vertex_list.erase(origin_vertex_list.begin() + end_j + 1, origin_vertex_list.begin() + start_j);
    }
    else{
        origin_vertex_list.erase(origin_vertex_list.begin() + end_j + 1, origin_vertex_list.end());
        origin_vertex_list.erase(origin_vertex_list.begin(), origin_vertex_list.begin() + start_j);
    }

    return true;

//
//    for (ll j = origin_range_start; ; )
//    {
//        ori_kept[j] = true;
//        if (j == origin_range_end) break;
//        j++;
//        if (j == origin_size) j = 0;
//    }










//
//
//
//    range.push_back(make_pair(start_i, end_i));
//    range_origin.push_back(make_pair(start_j, end_j));
//    ll f_start_i = start_i;
//
//    bool found = true;
//    while (found)
//    {
//        found = false;
//        //for (ll i = end_i + 1 ; i < f_start_i; i++)
//        ll i = end_i + 1;
//        while (i != f_start_i)
//        {
//            if (i == piece_size) {
//                i = -1;
//                continue;
//            }
//            for (ll j = origin_size - 1 ; j >= 0 ; j--)
//            {
//                if (piece->v_list[i] == origin->v_list[j])
//                {
//                    ll next_i = i + 1 == piece_size? 0 : i+1;
//                    ll next_j = j-1 == -1? origin_size-1 : j-1;
//                    if (piece->v_list[next_i] == origin->v_list[next_j])
//                    {
//                        middle_i = i;
//                        middle_j = j;
//                        found = true;
//                        break;
//                    }
//                }
//            }
//            if (found) break;
//            i++;
//        }
//        if (found){
//            findStartAndEnd(piece->v_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j);
//            range.push_back(make_pair(start_i, end_i));
//            range_origin.push_back(make_pair(start_j, end_j));
//        }
//    }
//
//    vector<Vertex*> origin_new;
//    vector<Vertex*> piece_new;
//
//    vector<bool> pie_kept(piece_size, false);
//    vector<bool> ori_kept(origin_size, false);
//
//    for (int i = 0 ; i < (ll)range.size() ; i++)
//    {
//        ll range_start = range[i].first;
//        ll range_end = range[i].second;
//
//        ll origin_range_start = range_origin[i].first;
//        ll origin_range_end = range_origin[i].second;
//
//        int seg_num = piece->getSegmentsNumber(range_end, range_start);
//        if (seg_num == 0)
//        {
//            /**< Only One Vertex Same*/
//            continue;
//        }
////
////        Point_3 sp = CGALCalculation::makePoint(piece->v_list[range_start]);
////        Point_3 ep = CGALCalculation::makePoint(piece->v_list[range_end]);
////        Line_3 line(sp, ep);
////
//        for (ll i = range_end;;)
//        {
//            //Point_3 newp = line.projection(CGALCalculation::makePoint(piece->v_list[i]));
//            //piece->v_list[i]->translateTo({newp.x(), newp.y(), newp.z()});
//            pie_kept[i] = true;
//            if (i == range_start) break;
//            i++;
//            if (i == piece->v_list.size()) i = 0;
//        }
//
//        for (ll j = origin_range_start; ; )
//        {
//            ori_kept[j] = true;
//            if (j == origin_range_end) break;
//            j++;
//            if (j == origin_size) j = 0;
//        }
//
//    }
//
//    for (ll i = 0 ; i < piece_size ; i++){
//        if (pie_kept[i]) piece_new.push_back(piece->v_list[i]);
//    }
//    for (ll i = 0 ; i < origin_size ; i++){
//        if (ori_kept[i]) origin_new.push_back(origin->v_list[i]);
//    }
//
//    origin->v_list.clear();
//    piece->v_list.clear();
//    piece->v_list = piece_new;
//    origin->v_list = origin_new;


    return 0;
}

