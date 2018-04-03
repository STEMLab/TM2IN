#include "SurfacePairComputation.h"
#include "VertexComputation.h"
#include "HalfEdgeComputation.h"
#include <stdlib.h>
#include <features/HalfEdge.h>

int SurfacePairComputation::combine(Surface* origin, Surface* piece, double degree) {
    // check Polygon is in near polygon or not
    if (!CanBeNeighbor(origin, piece)) return 1;

    if (!isNeighbor(origin, piece)) return 1;
/*
    if (origin->getVerticesSize() < piece->getVerticesSize()){
        Surface* temp = origin;
        origin = piece;
        piece = temp;
    }

    vector<int> shareEdgesIndices_piece = listShareEdgesInPiece(origin, piece);
    if (shareEdgesIndices_piece.size() != piece->getVerticesSize()){
        if (!Checker::CanbeMerged(origin->normal, piece->normal, degree)) {
            return 1;
        }
    }
    vector<int> shareEdgesIndices_origin = listShareEdgesInOrigin(nullptr, piece, shareEdgesIndices_piece);
*/

    ll piece_end = -1, origin_end = -1;
    ll piece_start = -1, origin_start = -1;
    ll piece_middle = -1, origin_middle = -1;

    ll origin_size = origin->getVerticesSize();
    ll piece_size = piece->getVerticesSize();
    vector<Vertex*> origin_vertex_list = origin->getVerticesList();
    vector<Vertex*> piece_vertex_list = piece->getVerticesList();

    if (!findShareVertex(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle)) return 1;
    if (CGALCalculation::getAngle(origin->normal, piece->normal) > 179.999999){
        return 1;
    }

    if (findStartAndEnd(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle, piece_start, piece_end, origin_start, origin_end)){
        cout << "\n" << origin->toJSONString() << endl;
        cout << "\n" << piece->toJSONString() <<endl;
        cout << CGALCalculation::getAngle(origin->normal, piece->normal)  << endl;
        return 1;
    }

    int seg_num = piece->getSegmentsNumber(piece_end, piece_start);

    if (seg_num == -1)
    {
        cerr << "segment Number is -1" << endl;
        exit(-1);
    }
    else if (seg_num == 0){
        return 1;
    }
    else if (seg_num == 1){

    }
    else{
        if (!Checker::CanbeMerged(origin->normal, piece->normal, degree)) {
            return 1;
        }
    }

    //find if another line share. make Hole?
    if (isMakingHole(piece_start, piece_end, origin_start, origin_end, piece_vertex_list, origin_vertex_list)) return 1;

    HalfEdgeComputation::setParent(piece->getBoundaryEdgesList(), origin);
    vector<HalfEdge*> new_edges;

    for (ll j = origin_start; ; ){
        new_edges.push_back(origin->boundary_edges(j));
        j++;
        if (j == origin_size) j = 0;
        if (j == origin_end) break;
    }
    for (ll i = piece_end; ;){
        new_edges.push_back(piece->boundary_edges(i));
        i++;
        if (i == piece_size) i = 0;
        if (i == piece_start) break;
    }

    origin->setBoundaryEdgesList(new_edges);
    origin->normal = origin->normal + piece->normal;
    origin->area += piece->area;
    origin->setMBB(piece);
    /*
    vector<Vertex*> new_v_list;

    for (ll j = origin_start; ; ){
        new_v_list.push_back(origin->v_list[j]);
        j++;
        if (j == origin_size) j = 0;
        if (j == origin_end) break;
    }
    for (ll i = piece_end; ;){
        new_v_list.push_back(piece->v_list[i]);
        i++;
        if (i == piece_size) i = 0;
        if (i == piece_start) break;

    }

    origin->v_list.clear();
    origin->v_list = new_v_list;
    origin->normal = origin->normal + piece->normal;
    origin->area += piece->area;
    origin->setMBB(piece);
*/

    if (origin->checkDuplicate()){
        cerr << "Duplicate" << endl;
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

bool SurfacePairComputation::doShareEdge(Surface *&surface_i, Surface *&surface_j){
    vector<HalfEdge*> i_edge_list = surface_i->getBoundaryEdgesList();
    for (HalfEdge* edge : i_edge_list){
        if (edge->oppositeEdge->parent == surface_j){
            return true;
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
    end_j = j;

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
    start_j = j;
    return 0;
}

bool SurfacePairComputation::CanBeNeighbor(Surface *cp1, Surface *cp2){
    return CGALCalculation::isIntersect_BBOX(cp1, cp2);
}

int SurfacePairComputation::simplifyLineSegment(Surface *origin, Surface *piece) {
    cerr << "TODO : simplifyLineSegment" << endl;
    ll middle_i = -1, middle_j = -1;
    ll piece_size = piece->getVerticesSize();
    ll origin_size = origin->getVerticesSize();

    vector<Vertex*> piece_vertex_list = piece->getVerticesList();
    vector<Vertex*> origin_vertex_list = origin->getVerticesList();

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
                    break;
                }
            }
        }
        if (middle_i != -1) break;
    }

    if (middle_i == -1) return 1;
    // if (CGALCalculation::getAngle(origin->normal, piece->normal) > 179.999999) return 1;

    ll end_i = -1, end_j = -1;
    ll start_i = -1, start_j = -1;
    if (findStartAndEnd(piece_vertex_list, origin_vertex_list, middle_i, middle_j, start_i, end_i, start_j, end_j)) return 1;

    assert (piece->getSegmentsNumber(start_i, end_i) > 1);

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

    origin->setVertexList(origin_vertex_list);
    piece->setVertexList(piece_vertex_list);

    return 0;
}


bool SurfacePairComputation::isNeighbor(Surface *origin, Surface *piece) {
    for (HalfEdge* he : origin->getBoundaryEdgesList()){
        if (he->oppositeEdge->parent == piece) return true;
    }
    return false;
}

vector<int> SurfacePairComputation::listShareEdgesInPiece(Surface *origin, Surface *piece) {
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

vector<int> SurfacePairComputation::listShareEdgesInOrigin(Surface *origin, Surface *piece, vector<int> share_edges) {
    vector<int> origin_edges;
    for (int i = 0 ; i < share_edges.size() ; i++){
        HalfEdge* he = piece->boundary_edges(share_edges[i]);
        int origin_index = origin->indexBoundaryEdge(he);
        origin_edges.push_back(origin_index);
    }
    return origin_edges;
}




