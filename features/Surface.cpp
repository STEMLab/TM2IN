//
//  Surface.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "features/Surface.h"

#include "logic/check.h"
#include "compute/Surface_pair_computation.h"
#include "compute/HalfEdgeComputation.h"
#include "compute/VertexComputation.h"
#include "features/Triangle.h"
#include "features/HalfEdge.h"
#include "space_maker/OnlyWallSpaceMaker.h"

#include <cstdlib>
#include <compute/SurfaceComputation.h>
#include <compute/VertexListComputation.h>
#include <cgal/Features_to_CGAL_object.h>


using namespace std;

Surface::Surface(Surface* cp){
    this->boundaryEdges = cp->boundaryEdges;
    HalfEdgeComputation::setParent(this->boundaryEdges, this);

    this->normal = cp->normal;
    for (int i = 0 ; i < 3 ; i++){
        this->max_coords[i] = cp->max_coords[i];
        this->min_coords[i] = cp->min_coords[i];
    }
    this->area = cp->area;
    this->triangles = cp->triangles;
    this->sf_id = cp->sf_id;
}

Surface::Surface(std::vector<Vertex*>& pVertices){
    cerr << "TODO" << endl;
}

Surface::Surface(Triangle& pl){
    this->boundaryEdges = pl.boundaryEdges;
    HalfEdgeComputation::setParent(this->boundaryEdges, this);

    area = pl.getArea();
    normal = pl.getNormal();

    this->updateMBB();
    this->triangles.push_back(&pl);
}


Vector_3 Surface::getNormal()
{
    return this->normal;
}

double Surface::getArea(){
    return this->area;
}

void Surface::updateMBB(){
    for (int i = 0 ; i < 3 ; i++)
    {
        this->max_coords[i] = -10000000.000;
        this->min_coords[i] = 10000000.00;
    }

    for (unsigned int i = 0 ; i < this->getVerticesSize() ; i++){
        for (int j = 0 ; j < 3 ; j++){
            this->max_coords[j] = max(this->max_coords[j], this->vertex(i)->coords[j]);
            this->min_coords[j] = min(this->min_coords[j], this->vertex(i)->coords[j]);
        }
    }
}

void Surface::setMBB(Surface* cp){
    for (int j = 0 ; j < 3 ; j++){
        this->max_coords[j] = max(this->max_coords[j], cp->max_coords[j]);
        this->min_coords[j] = min(this->min_coords[j], cp->min_coords[j]);
    }

}

void Surface::translate(double diff[]){
    for (int j = 0 ; j < 3 ; j++){
        this->max_coords[j] += diff[j];
        this->min_coords[j] += diff[j];
    }
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

// TODO : move validator
bool Surface::checkDuplicate(){
    vector<Vertex*> vertexList = this->getVerticesList();
    return VertexListComputation::checkDuplicate(vertexList);
}

string Surface::asJsonText(){
    cerr << "asJSONTEXT" <<endl;
}

std::string Surface::toJSONWithTriangles() {

}

//TODO : move
Vector_3 Surface::getSimpleNormal(){
    Vector_3 normal = Vector_3(0,0,0);
    for (int i = 0 ; i < (int) this->getVerticesSize() - 1 ; i += 2){
        int e_i = i + 2 >= (int) this->getVerticesSize()? 0 : i+2;
        normal = normal + CGALCalculation::getCrossProduct(vertex(i), vertex(i+1), vertex(e_i));
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
        Surface* thisSurface = this;
        int triangulate = SurfaceComputation::triangulate(thisSurface, triangles);
        if (triangulate) return true;

        Vector_3 sumNormal(0,0,0);
        for (Triangle* tri : triangles){
            sumNormal += tri->normal;
            delete tri;
        }
        this->normal = sumNormal;
    }
//    this->normal = this->normal / sqrt(this->normal.squared_length());
//    this->normal = this->normal * this->area * AREA_CONST;

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

bool Surface::isInMBB(Vertex* vt){
    if (vt->x() >= this->min_coords[0] && vt->x() <= this->max_coords[0]){
        if (vt->y() >= this->min_coords[1] && vt->y() <= this->max_coords[1]){
            if (vt->z() >= this->min_coords[2] && vt->z() <= this->max_coords[2]){
                return true;
            }
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

bool Surface::compareArea(Surface* i, Surface* j) {
     return (i->area > j->area);
}


/**
*  Check that Surface is not valid. Straight Line or Point.
*/
// TODO : move
bool Surface::isValid(){
    if (this->getVerticesSize() < 3) {
        return false;
    }
    if (this->checkDuplicate()) return false;
    return true;
}

Point_3 Surface::findLowestPoint(){
    Vertex* cent = SurfaceComputation::getCenterPoint(this);
    Plane_3 plane(CGAL_User::getCGALPoint(cent), this->normal);
    delete cent;

    double max_dist = -1.0;
    int max_index = 0;
    for (ull index= 0 ; index < this->getVerticesSize() ; index++){
        Point_3 p = CGAL_User::getCGALPoint(this->vertex(index));
        if (plane.oriented_side(p) != CGAL::ON_POSITIVE_SIDE){
            double dist = CGAL::squared_distance(plane, p);
            if (dist > max_dist){
                max_dist = dist;
                max_index = index;
            }
        }
    }
    return CGAL_User::getCGALPoint(this->vertex(max_index));
}

Plane_3 Surface::getPlaneWithLowest(){
    Point_3 point = findLowestPoint();
    return Plane_3(point, this->normal);
}

vector<Vertex *> Surface::getVerticesList() {
    return HalfEdgeComputation::getFirstVertexList(this->boundaryEdges);
}

void Surface::setVertexList(std::vector<Vertex *> newVertices) {
    this->boundaryEdges.clear();
    for (int i = 0 ; i < newVertices.size() - 1; i++){
        Vertex* v1 = newVertices[i];
        Vertex* v2 = newVertices[i+1];
        this->boundaryEdges.push_back(new HalfEdge(v1, v2, this));
    }
    this->boundaryEdges.push_back(new HalfEdge(newVertices[newVertices.size() - 1], newVertices[0], this));
}

std::vector<HalfEdge *> Surface::getBoundaryEdgesList() {
    if (boundaryEdges.size() == 0) cerr << "no boundary" << endl;
    return boundaryEdges;
}

void Surface::setBoundaryEdgesList(std::vector<HalfEdge*> edges){
    this->boundaryEdges = edges;
}

void Surface::clearTriangleList() {
    this->triangles.clear();
}

/**
 * remove [startIndex, endIndex) in Vertex List.
 * @param startIndex
 * @param endIndex
 */
void Surface::removeVertexByIndex(int startIndex, int endIndex) {
    // this->v_list.erase(this->v_list.begin() + startIndex, this->v_list.begin() + endIndex);
    cerr << "TODO" << endl;
}

void Surface::setVertex(int index, Vertex *vt) {
    this->boundaryEdges[index]->setVertex(0, vt);
    if (index == 0)
        this->boundaryEdges[this->getVerticesSize() - 1]->setVertex(1, vt);
    else
        this->boundaryEdges[index - 1]->setVertex(1, vt);
}

void Surface::insertVertex(int index, Vertex *vt) {
    // this->v_list.insert(this->v_list.begin() + index, vt);
    cerr << "TODO : insertVertex" << endl;
}

void Surface::setPlaneRef(Plane_3 pl) {
    this->planeRef = pl;
}

Vertex *Surface::vertex(int index) {
    return this->boundaryEdges[index]->vertices[0];
}

HalfEdge *Surface::boundary_edges(int i) {
    return this->boundaryEdges[i];
}

int Surface::indexBoundaryEdge(HalfEdge *pEdge) {
    int i = 0;
    for (HalfEdge* he : this->boundaryEdges){
        if (pEdge == he) return i;
        i++;
    }
    return -1;
}

void Surface::removeVertexByIndex(int id) {
    cerr << "TODO : remove Surface::removeVertexByIndex" << endl;
}

std::ostream& operator<<(std::ostream &ou, Surface *pSurface) {
    ou << pSurface->asJsonText() << endl;
    return ou;
}
