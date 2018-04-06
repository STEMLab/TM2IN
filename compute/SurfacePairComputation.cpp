#include "SurfacePairComputation.h"
#include "VertexComputation.h"
#include "HalfEdgeComputation.h"
#include "VertexListComputation.h"
#include <stdlib.h>
#include <features/HalfEdge.h>

int SurfacePairComputation::combine(Surface* origin, Surface* piece, double degree) {
    // check Polygon is in near polygon or not
    if (!CGALCalculation::isIntersect_BBOX(origin, piece)) return 1;

    // check They are neighbor
    if (!isNeighbor(origin, piece)) return 1;

    ll origin_size = origin->getVerticesSize();
    ll piece_size = piece->getVerticesSize();
    vector<Vertex*> origin_vertex_list = origin->getVerticesList();
    vector<Vertex*> piece_vertex_list = piece->getVerticesList();

    ll piece_middle = -1, origin_middle = -1;
    if (!findShareVertex(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle)) return 1;
    if (CGALCalculation::getAngle(origin->normal, piece->normal) > 179.999999){
        return 1;
    }

    ll lastVertex_piece = -1, lastVertex_origin = -1;
    ll firstVertex_piece = -1, firstVertex_origin = -1;
    if (findStartAndEnd(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle, firstVertex_piece, lastVertex_piece, firstVertex_origin, lastVertex_origin)){
        cerr << "\n" << origin->toJSONString() << endl;
        cerr << "\n" << piece->toJSONString() <<endl;
        cerr << CGALCalculation::getAngle(origin->normal, piece->normal)  << endl;
        return 1;
    }

    assert (piece->vertex(firstVertex_piece) == origin->vertex(lastVertex_origin));
    assert (piece->vertex(lastVertex_piece) == origin->vertex(firstVertex_origin));

    int seg_num = piece->getSegmentsNumber(lastVertex_piece, firstVertex_piece);

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

    HalfEdgeComputation::setParent(piece->getBoundaryEdgesList(), origin);
    vector<HalfEdge*> new_edges;

    for (ll j = lastVertex_origin; ; ){
        new_edges.push_back(origin->boundary_edges(j));
        j++;
        if (j == origin_size) j = 0;
        if (j == firstVertex_origin) break;
    }

    for (ll i = lastVertex_piece; ;){
        new_edges.push_back(piece->boundary_edges(i));
        i++;
        if (i == piece_size) i = 0;
        if (i == firstVertex_piece) break;
    }

    vector<Vertex*> newVertexList = HalfEdgeComputation::getFirstVertexList(new_edges);
    if (VertexListComputation::checkDuplicate(newVertexList)) return 1;

    origin->setBoundaryEdgesList(new_edges);
    origin->normal = origin->normal + piece->normal;
    origin->area += piece->area;
    origin->setMBB(piece);

    // assert (!origin->checkDuplicate());

    //TODO : delete old edges

    // delete piece;

    return 0;
}


int SurfacePairComputation::simplifyLineSegment(Surface *origin, Surface *piece) {
    ll piece_middle = -1, origin_middle = -1;
    ll piece_size = piece->getVerticesSize();
    ll origin_size = origin->getVerticesSize();

    vector<Vertex*> piece_vertex_list = piece->getVerticesList();
    vector<Vertex*> origin_vertex_list = origin->getVerticesList();

    for (ll i = 0 ; i < piece_size ;i++){
        for (ll j = origin_size - 1 ; j >= 0 ; j--){
            if (piece_vertex_list[i] == origin_vertex_list[j]){
                ll next_i = i + 1 == piece_size? 0 : i+1;
                ll next_j = j-1 == -1? origin_size-1 : j-1;

                ll pre_i = i - 1 == -1? piece_size-1 : i-1;
                ll pre_j = j + 1 == origin_size? 0 : j + 1;

                if (piece_vertex_list[next_i] == origin_vertex_list[next_j]
                    && piece_vertex_list[pre_i] == origin_vertex_list[pre_j]){
                    piece_middle = i;
                    origin_middle = j;
                    break;
                }
            }
        }
        if (piece_middle != -1) break;
    }

    if (piece_middle == -1) return 1;

    ll lastVertex_piece = -1, lastVertex_origin = -1;
    ll firstVertex_piece = -1, firstVertex_origin = -1;
    if (findStartAndEnd(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle, firstVertex_piece, lastVertex_piece, firstVertex_origin, lastVertex_origin)) return 1;

    assert (piece->getSegmentsNumber(firstVertex_piece, lastVertex_piece) > 1);
    assert (piece->vertex(firstVertex_piece) == origin->vertex(lastVertex_origin));
    assert (piece->vertex(lastVertex_piece) == origin->vertex(firstVertex_origin));

    HalfEdge* newEdge_piece = new HalfEdge(piece->vertex(firstVertex_piece), piece->vertex(lastVertex_piece), piece);
    HalfEdge* newEdge_origin = new HalfEdge(origin->vertex(firstVertex_origin), origin->vertex(lastVertex_origin), origin);
    newEdge_origin->setOppositeEdge(newEdge_piece);
    newEdge_piece->setOppositeEdge(newEdge_origin);

    vector<HalfEdge*> newHalfEdgeList_piece;
    vector<HalfEdge*> newHalfEdgeList_origin;

    for (ll j = lastVertex_origin; ; ){
        newHalfEdgeList_origin.push_back(origin->boundary_edges(j));
        j++;
        if (j == origin_size) j = 0;
        if (j == firstVertex_origin) break;
    }
    newHalfEdgeList_origin.push_back(newEdge_origin);

    for (ll j = lastVertex_piece; ; ){
        newHalfEdgeList_piece.push_back(piece->boundary_edges(j));
        j++;
        if (j == piece_size) j = 0;
        if (j == firstVertex_piece) break;
    }
    newHalfEdgeList_piece.push_back(newEdge_piece);

    origin->setBoundaryEdgesList(newHalfEdgeList_origin);
    piece->setBoundaryEdgesList(newHalfEdgeList_piece);

    //if piece become line.(only has two vertex)
    if (piece->getVerticesSize() < 3){
        assert(piece->getVerticesSize() == 2);
        newEdge_origin->setOppositeEdge(newHalfEdgeList_piece[0]->getOppositeEdge());
        newHalfEdgeList_piece[0]->getOppositeEdge()->setOppositeEdge(newEdge_origin);
    }

    return 0;
}

int SurfacePairComputation::doShareEdge(Surface *&surface_i, Surface *&surface_j){
    vector<HalfEdge*> iEdgeList = surface_i->getBoundaryEdgesList();
    int share_edge_count = 0;
    for (HalfEdge* edge : iEdgeList){
        if (edge->oppositeEdge->parent == surface_j){
            share_edge_count++;
        }
    }

    return share_edge_count;
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




