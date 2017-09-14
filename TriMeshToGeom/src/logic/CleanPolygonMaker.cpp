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


<<<<<<< HEAD
int CleanPolygonMaker::simplifyLineSegment(Surface* origin, Surface* piece){
=======
/** < simplify only overlap part of line*/
int CleanPolygonMaker::simplifyShareLine(CombinedPolygon* origin, CombinedPolygon* piece, Checker* checker){
    if (!isNeighbor(origin, piece)) return false;

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
>>>>>>> 57641960d303c0103fe9d38e0b09179625eff96d
    ll middle_i = -1, middle_j = -1;
    ll piece_size = piece->getLength();
    ll origin_size = origin->getLength();

    if (!findShareVertex(piece->v_list, origin->v_list, middle_i, middle_j)) return false;
<<<<<<< HEAD

    vll range;
    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    findStartAndEnd(piece->v_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j);
    range.push_back(make_pair(start_i, end_i));
    ll f_start_i = start_i;

    bool found = true;
    while (found)
    {
        found = false;
        //for (ll i = end_i + 1 ; i < f_start_i; i++)
        ll i = end_i + 1;
        while (i != f_start_i)
        {
            if (i == piece_size) {
                i = -1;
                continue;
=======
    
    /**< [start_i, end_i] */
    findStartAndEnd(piece->v_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j);

    
    int seg_num = piece->getSegmentsNumber(start_i, end_i);
    if (seg_num == -1)
    {
        exit(-1);
    }
    else if (seg_num == 0){
        /**< Only One Vertex Same*/
        return 0;
    }
    
    vector<Vertex*> simple = simplifySegment(piece->v_list, start_i, end_i, checker);
    vector<Vertex*> origin_new_vlist;
    vector<Vertex*> piece_new_vlist;
    
    for (ll j = start_j; ; ){
        origin_new_vlist.push_back(origin->v_list[j]);
        j++;
        if (j == origin_size) j = 0;
        if (j == end_j) break;
    }
    for (ll j = 0 ; j < simple.size() ; j++){
        origin_new_vlist.push_back(simple[j]);
    }
    
    for (ll i = end_i; ;){
        piece_new_vlist.push_back(piece->v_list[i]);
        i++;
        if (i == piece_size) i = 0;
        if (i == start_i) break;
    }
    for (ll j = 0 ; j < simple.size() ; j++){
        piece_new_vlist.push_back(simple[j]);
    }
    
    origin->v_list.clear();
    piece->v_list.clear();
    
    origin->v_list = origin_new_vlist;
    piece->v_list = piece_new_vlist;
    
    return seg_num;
}

vector<Vertex*> CleanPolygonMaker::simplifySegment(vector<Vertex*>& origin, ll start, ll end, Checker* checker){
    vector<Vertex*> simple, temp;
    vector<ll> vertex_list;
    
    ll origin_size = origin.size();
    
    for (ll index = start; index != end; index++){
        if (index == origin_size){
            index = -1;
            continue;
        }
        temp.push_back(origin[index]);
    }
    
    bool ischanged = true;
    vertex_list.push_back(0);
    vertex_list.push_back(temp.size() - 1);
    
    while(ischanged){
        ischanged = false;
        for (ll i = 0; i < vertex_list.size() - 1 ; i++){
            ll ni = i + 1;
            ll si = vertex_list[i];
            ll ei = vertex_list[ni];
            for (ll j = si + 1 ; j < ei ; j++){
                if (!checker->isSamePlanar(temp[si], temp[ei], temp[j])){
                    vertex_list.push_back(j);
                    ischanged=true;
                }
>>>>>>> 57641960d303c0103fe9d38e0b09179625eff96d
            }
            for (ll j = origin_size - 1 ; j >= 0 ; j--)
            {
                if (piece->v_list[i] == origin->v_list[j])
                {
                    ll next_i = i + 1 == piece_size? 0 : i+1;
                    ll next_j = j-1 == -1? origin_size-1 : j-1;
                    if (piece->v_list[next_i] == origin->v_list[next_j])
                    {
                        middle_i = i;
                        middle_j = j;
                        found = true;
                        break;
                    }
                }
            }
            if (found) break;
            i++;
        }
        if (found){
            findStartAndEnd(piece->v_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j);
            range.push_back(make_pair(start_i, end_i));
        }
<<<<<<< HEAD
=======
        sort(vertex_list.begin(), vertex_list.end());
>>>>>>> 57641960d303c0103fe9d38e0b09179625eff96d
    }
    
    for (ll index = 0 ; index < vertex_list.size() ; index++){
        simple.push_back(temp[vertex_list[index]]);
    }
    
    temp.clear();

<<<<<<< HEAD
    for (int i = 0 ; i < range.size() ; i++)
    {
        ll range_start = range[i].first;
        ll range_end = range[i].second;
        int seg_num = piece->getSegmentsNumber(range_end, range_start);
        if (seg_num == 0)
        {
            /**< Only One Vertex Same*/
            return false;
        }

        Point_3 sp = CGALCalculation::makePoint(piece->v_list[range_start]);
        Point_3 ep = CGALCalculation::makePoint(piece->v_list[range_end]);
        Line_3 line(sp, ep);

        for (ll i = range_start;;)
        {
            Point_3 newp = line.projection(CGALCalculation::makePoint(piece->v_list[i]));
            piece->v_list[i]->translateTo({newp.x(), newp.y(), newp.z()});
            i++;
            if (i == piece_size) i = 0;
            if (i == range_end) break;
        }
    }


//    origin->setMBB();
//    origin->refreshNormal();
//
//    piece->setMBB();
//    piece->refreshNormal();


    return 0;
=======
    return simple;
>>>>>>> 57641960d303c0103fe9d38e0b09179625eff96d
}

