//
//  Surface.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "features/Surface.h"

#include "features/Triangle.h"
#include "features/HalfEdge.h"

#include <cstdlib>
#include <compute/unused.h>
#include <algorithm/triangulation.h>
#include <algorithm/compare.h>
#include <detail/features/halfedge_string.h>
#include <cgal/triangle.h>
#include "detail/io/JsonWriter.h"
#include "detail/algorithm/self_intersect.h"

using namespace std;

namespace TM2IN{
    Surface::Surface(Surface* cp){
        this->exteriorBoundary = cp->exteriorBoundary;
        TM2IN::detail::HalfEdgeString::setParent(this->exteriorBoundary, this);
        this->normal = cp->normal;
        this->mbb = new MinimumBoundingBox(cp->mbb);
        this->area = cp->area;
        this->triangles = cp->triangles;
        this->sf_id = cp->sf_id;
        this->type = TM2IN::IND_TYPE::Surface;
    }

    Surface::Surface(Triangle& pl){
        this->exteriorBoundary = pl.exteriorBoundary;
        TM2IN::detail::HalfEdgeString::setParent(this->exteriorBoundary, this);

        area = pl.getArea();
        normal = pl.getNormal();

        this->updateMBB();
        this->triangles.push_back(&pl);
        this->type = TM2IN::IND_TYPE::Surface;
    }


    Vector_3 Surface::getNormal()
    {
        return this->normal;
    }

    void Surface::updateMBB() {
        double max_coords[3] = {0};
        double min_coords[3] = {0};
        for (int i = 0 ; i < 3 ; i++)
        {
            max_coords[i] = -10000000.000;
            min_coords[i] = 10000000.00;
        }

        for (unsigned int i = 0 ; i < this->getVerticesSize() ; i++){
            for (int j = 0 ; j < 3 ; j++){
                max_coords[j] = max(max_coords[j], this->vertex(i)->coords[j]);
                min_coords[j] = min(min_coords[j], this->vertex(i)->coords[j]);
            }
        }
        this->mbb->set_min_coords(min_coords);
        this->mbb->set_max_coords(max_coords);
    }

    int Surface::getSegmentsNumber(ll si, ll ei) {
        if (si >= this->getVerticesSize() || ei >= getVerticesSize()){
            cerr << "getSegmentsNumber Error" << endl;
            return -1;
        }
        if (ei >= si){
            return ei - si;
        }
        else{
            return this->getVerticesSize() - si + ei;
        }
    }

    /**
    *  Check that Surface is not valid. Straight Line or Point.
    */
    bool Surface::strict_validation(){
        if (!easy_validation()) {
            return false;
        }
        return this->is_simple();
    }

    bool Surface::easy_validation(){
        if (this->getVerticesSize() < 3) {
            return false;
        }
        return !this->has_duplicate_vertex();
    }

    /**
     * Check surface simple (existence of self-intersection).
     * @return Is this surface simple?
     */
    bool Surface::is_simple(){
        if (has_duplicate_vertex()) return false;
        return !TM2IN::detail::algorithm::has_self_intersection(this->getExteriorBoundary());
    }


    bool Surface::has_duplicate_vertex(){
        vector<Vertex*> sorted_v_list(this->getVerticesList());
        sort(sorted_v_list.begin(), sorted_v_list.end(), TM2IN::algorithm::greater);
        for (ull i = 0 ; i < sorted_v_list.size() - 1; i++){
            if (sorted_v_list[i] == sorted_v_list[i+1]){
                return true;
            }
        }
        return false;
    }

    string Surface::asJsonText(){
        return TM2IN::detail::io::surface_to_json(*this);
    }

    //TODO : move
    Vector_3 Surface::getSimpleNormal(){
        Vector_3 normal = Vector_3(0,0,0);
        for (int i = 0 ; i < (int) this->getVerticesSize() - 1 ; i += 2){
            int e_i = i + 2 >= (int) this->getVerticesSize()? 0 : i+2;
            normal = normal + TM2IN::cgal::triangle_normal(vertex(i), vertex(i + 1), vertex(e_i));
        }
        return normal;
    }

    //TODO : move
    bool Surface::updateNormal(){
        if (this->getVerticesSize() <= 4){
            this->normal = getSimpleNormal();
        }
        else{
            vector<Triangle*> triangles;
            int triangulate = TM2IN::algorithm::triangulate(this, triangles);
            if (triangulate) return true;

            Vector_3 sumNormal(0,0,0);
            for (Triangle* tri : triangles){
                sumNormal += tri->normal;
                delete tri;
            }
            this->normal = sumNormal;
        }

        assert(this->normal != CGAL::NULL_VECTOR);
        return true;
    }

    // TODO : move
    bool Surface::isOpposite(Surface* sf){
        for (ll i = 0 ; i < (ll) sf->getVerticesSize() ; i++){
            if (this->vertex(0) == sf->vertex(i)){
                ll sf_index = i + 1;
                if (sf_index == (ll) sf->getVerticesSize()) sf_index = 0;
                ll this_index = this->getVerticesSize() - 1;
                while (this->vertex(this_index) == sf->vertex(sf_index)){
                    this_index--; sf_index++;
                    if (sf_index == (ll) sf->getVerticesSize()) sf_index = 0;
                    if (this_index == 0 || sf_index == i) break;

                }
                if (this->vertex(this_index) != sf->vertex(sf_index))
                    return false;
                else
                    return true;

            }
        }
        return false;
    }

    /**
     * For Sorting.
     *
     */
    bool Surface::compareLength(Surface* i, Surface* j) {
        return (i->getVerticesSize() > j->getVerticesSize());
    }

    vector<Vertex *> Surface::getVerticesList() {
        return TM2IN::detail::HalfEdgeString::getFirstVertexList(this->exteriorBoundary);
    }

    void Surface::setVertexList(std::vector<TM2IN::Vertex *> newVertices) {
        this->exteriorBoundary.clear();
        for (int i = 0 ; i < newVertices.size() - 1; i++){
            TM2IN::Vertex* v1 = newVertices[i];
            TM2IN::Vertex* v2 = newVertices[i+1];
            this->exteriorBoundary.push_back(new HalfEdge(v1, v2, this));
        }
        this->exteriorBoundary.push_back(new HalfEdge(newVertices[newVertices.size() - 1], newVertices[0], this));
    }

    std::vector<HalfEdge *> Surface::getExteriorBoundary() {
        if (exteriorBoundary.size() == 0) cerr << "no boundary" << endl;
        return exteriorBoundary;
    }

    void Surface::setExteriorBoundary(std::vector<HalfEdge *> edges){
        this->exteriorBoundary.clear();
        this->exteriorBoundary = edges;
        /*
        for (HalfEdge* edge : edges){
            edge->setParent(this);
        }
         */
    }

    void Surface::setVertex(int index, Vertex *vt) {
        this->exteriorBoundary[index]->setVertex(0, vt);
        if (index == 0)
            this->exteriorBoundary[this->getVerticesSize() - 1]->setVertex(1, vt);
        else
            this->exteriorBoundary[index - 1]->setVertex(1, vt);
    }

    void Surface::insertVertex(int index, Vertex *vt) {
        // this->v_list.insert(this->v_list.begin() + index, vt);
        cerr << "TODO : insertVertex" << endl;
    }

    Vertex *Surface::vertex(int index) {
        return this->exteriorBoundary[index]->vertices[0];
    }

    HalfEdge *Surface::exterior_boundary_edge(int i) {
        return this->exteriorBoundary[i];
    }

    int Surface::index_of_exterior_boundary_edge(HalfEdge *pEdge) {
        int i = 0;
        for (HalfEdge* he : this->exteriorBoundary){
            if (pEdge == he) return i;
            i++;
        }
        return -1;
    }

    std::ostream& operator<<(std::ostream &ou, Surface *pSurface) {
        ou << pSurface->asJsonText() << endl;
        return ou;
    }

    std::vector<Triangle *> Surface::getTriangulation() {

        if (this->triangulation.size() == 0) {
            vector<Triangle*> triList;
            TM2IN::algorithm::triangulate(this, triList);
        }
        return this->triangulation;
    }

    Surface::Surface() {
        type = TM2IN::IND_TYPE ::Surface;
    }

}
