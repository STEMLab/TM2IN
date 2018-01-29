#include "SurfacePairComputation.h"
#include "VertexComputation.h"
#include <stdlib.h>

int SurfacePairComputation::combine(Surface* origin, Surface* piece, double degree){
     // check Polygon is in near polygon or not
    if (!isNeighbor(origin, piece)) return 1;

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    ll middle_i = -1, middle_j = -1;

    ll origin_size = origin->sizeOfVertices();
    ll piece_size = piece->sizeOfVertices();

    if (!findShareVertex(piece->v_list, origin->v_list, middle_i, middle_j)) return 1;
    if (CGALCalculation::getAngle(origin->av_normal, piece->av_normal) > 179.999999){
        return 1;
    }
    /**< [start_i, end_i], [end_j, start_j] */
    if (findStartAndEnd(piece->v_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j)){
        cout << "\n" << origin->toJSONString() << endl;
        cout << "\n" << piece->toJSONString() <<endl;
        cout << CGALCalculation::getAngle(origin->av_normal, piece->av_normal)  << endl;
        return 1;
    }

    int seg_num = piece->getSegmentsNumber(end_i, start_i);


    if (seg_num == -1)
    {
        exit(-1);
    }
    else if (seg_num == 0){
        /**< Only One Vertex Same*/
        return 1;
    }
    else if (seg_num == 1){

    }
    else{
        if (!Checker::CanbeMerged(origin->av_normal, piece->av_normal, degree)) {
            return 1;
        }
    }

    //find if another line share. make Hole?
    if (isMakingHole(start_i, end_i, start_j, end_j, piece->v_list, origin->v_list)) return 1;

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
    origin->area += piece->area;
    origin->setMBB(piece);

    if (origin->checkDuplicate()){
        cout << "Duplicate" << endl;
        exit(-1);
    }
    return 0;
}

bool SurfacePairComputation::isMakingHole(ll start_i, ll end_i, ll start_j, ll end_j, vector<Vertex*>& piece_v_list, vector<Vertex*>& origin_v_list)
{
    ll piece_size = piece_v_list.size();
    ll origin_size = origin_v_list.size();

    for (ll i = end_i + 1 ; i != start_i ; i++)
    {
        if (i == piece_size)
        {
            i = -1;
            continue;
        }
        for (ll j = end_j - 1 ; j != start_j ; j--)
        {
            if (j == -1)
            {
                j = origin_size;
                continue;
            }

            if (piece_v_list[i] == origin_v_list[j])
            {
                return true;
            }
        }
    }
    return false;
}

bool SurfacePairComputation::isShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj){
    ll piece_size = vi.size();
    ll origin_size = vj.size();


    for (ll i = 0 ; i < piece_size ;i++){
        for (ll j = origin_size - 1 ; j >= 0 ; j--){
            if (vi[i] == vj[j]){
                ll next_i = i + 1 == piece_size? 0 : i+1;
                ll next_j = j-1 == -1? origin_size-1 : j-1;
                if (vi[next_i] == vj[next_j]){
                    return true;
                }
            }
        }
    }

    return false;
}


bool SurfacePairComputation::findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j){
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

int SurfacePairComputation::findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j){
    ll piece_size = vi.size();
    ll origin_size = vj.size();

    ll i = middle_i, j = middle_j;

    ll next_i = i + 1 == piece_size? 0 : i+1;
    ll next_j = j-1 == -1? origin_size-1 : j-1;

    ll num = 0;
    while (vi[next_i] == vj[next_j])
    {
        i = next_i;
        j = next_j;

        next_i = i + 1 == piece_size? 0 : i+1;
        next_j = j-1 == -1? origin_size-1 : j-1;

        num++;
        if ((num > vi.size() + 1) &&(num > vj.size() + 1)){ //maybe same surface but opposite
            cout << "infinite loop in find Start And End" << endl;
            return 1;
        }
    }
    end_i = i;
    end_j = j;

    i = middle_i;
    j = middle_j;

    next_i = i - 1 == -1? vi.size() -1 : i - 1;
    next_j = j + 1 == origin_size? 0 : j + 1;

    num = 0;
    while (vi[next_i] == vj[next_j])
    {
        i = next_i;
        j = next_j;

        next_i = i - 1 == -1? vi.size() -1 : i - 1;
        next_j = j + 1 == origin_size? 0 : j + 1;
        num++;
        if ((num > vi.size() + 1) && (num > vj.size() + 1)){//maybe same surface but opposite
            cout << "infinite loop in find Start And End" << endl;
            return 1;
        }
    }
    start_i = i;
    start_j = j;
    return 0;
}

bool SurfacePairComputation::isNeighbor(Surface* cp1, Surface* cp2){
    return CGALCalculation::isIntersect_BBOX(cp1, cp2);
}

int SurfacePairComputation::simplifyLineSegment(Surface* origin, Surface* piece, bool again){
    if (!again && !isNeighbor(origin, piece)) return 1;

    ll middle_i = -1, middle_j = -1;
    ll piece_size = piece->sizeOfVertices();
    ll origin_size = origin->sizeOfVertices();

    vector<Vertex*>& piece_vertex_list = piece->v_list;
    vector<Vertex*>& origin_vertex_list = origin->v_list;

    bool hasTwoShareLine = false;
    ll infinite_check = 0;
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

    if (!hasTwoShareLine) return 1;
    if (CGALCalculation::getAngle(origin->av_normal, piece->av_normal) > 179.999999){
        return 1;
    }

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    if (findStartAndEnd(piece_vertex_list, origin->v_list, middle_i, middle_j, start_i, end_i, start_j, end_j)){
        return 1;
    }

    int seg_num = piece->getSegmentsNumber(start_i, end_i);
    if (seg_num <= 1) {
        cout << "simplifyLineSegment Errrrrror" << endl;
        exit(-1);
    }

    Point_3 sp = VertexComputation::getCGALPoint(piece_vertex_list[start_i]);
    Point_3 ep = VertexComputation::getCGALPoint(piece_vertex_list[end_i]);
    Line_3 line(sp, ep);

    //Translate to make it straight
    for (ll i = start_i + 1;;)
    {
        infinite_check++;
        assert (infinite_check < piece_size + 100) ;

        if (i == (ll)piece_vertex_list.size()) i = 0;
        if (i == end_i) break;
        Point_3 newp = line.projection(VertexComputation::getCGALPoint(piece_vertex_list[i]));
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

    return 0;
}

bool SurfacePairComputation::doIntersect(Surface *sp1, Surface *sp2) {
    return false;
}


