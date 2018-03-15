//
//  Surface.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "logic/check.hpp"
#include "features/Surface.h"
#include "compute/SurfacePairComputation.h"
#include "compute/VertexComputation.h"
#include "predefine.h"
#include "space_maker/OnlyWallSpaceMaker.h"

#include <cstdlib>
#include <compute/SurfaceComputation.h>
#include <compute/VertexListComputation.h>


using namespace std;

Surface::Surface(Surface* cp){
    this->v_list = cp->v_list;
    this->normal = cp->normal;
    for (int i = 0 ; i < 3 ; i++){
        this->max_coords[i] = cp->max_coords[i];
        this->min_coords[i] = cp->min_coords[i];
    }
    this->area = cp->area;
    this->triangles = cp->triangles;
}

Surface::Surface(std::vector<Vertex*>& pVertices){
    this->v_list = pVertices;
    this->updateMBB();
}

Surface::Surface(Triangle& pl){
    Vertex* v[3] = {pl[0], pl[1], pl[2]};

    for (int i = 0 ; i < 3 ;i++){
        v_list.push_back(v[i]);
    }

    area = pl.getArea();
    normal = pl.getNormal();

    this->updateMBB();
    this->triangles.push_back(&pl);
}

void Surface::setZ(double value){
    for (ull i = 0 ; i < this->getVerticesSize() ; i++){
        v_list[i]->setZ(value);
    }
}

void Surface::updateMBB(){
    for (int i = 0 ; i < 3 ; i++)
    {
        this->max_coords[i] = -10000000.000;
        this->min_coords[i] = 10000000.00;
    }

    for (unsigned int i = 0 ; i < this->getVerticesSize() ; i++){
        for (int j = 0 ; j < 3 ; j++){
            this->max_coords[j] = max(this->max_coords[j], this->v_list[i]->coords[j]);
            this->min_coords[j] = min(this->min_coords[j], this->v_list[i]->coords[j]);
        }
    }
}


void Surface::setMBB(Triangle* pl){
    Surface* sf = new Surface(pl);
    this->setMBB(sf);
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

bool Surface::checkDuplicate(){
    vector<Vertex*> sorted_v_list(this->v_list);

    sort(sorted_v_list.begin(), sorted_v_list.end(), VertexComputation::greater);
    for (ull i = 0 ; i < sorted_v_list.size() - 1; i++){
        if (sorted_v_list[i] == sorted_v_list[i+1]){
            cout << "\nSame Index" << endl;
            return true;
        }

        /*
        if (ch != NULL && ch->isSameVertex(sorted_v_list[i], sorted_v_list[i+1])){
            cout << "\nSame Coords" << endl;
            cout << sorted_v_list[i]->toJSON() << endl;
            cout << sorted_v_list[i+1]->toJSON() <<endl;
            return true;
        }
         */
    }
    return false;
}

string Surface::toJSONString(){
    if (this->getVerticesSize() == 0){
        cerr << "Vertex Length is 0" << endl;
        return "";
    }
    string ret;
    ret.append("{");
    ret.append(" \n \"area\" : " + to_string(area) );
    ret.append(" \n, \"id\" : " + to_string(sf_id) );
    ret.append(" \n, \"normal\" : [");
    ret.append(to_string(this->normal.x()) + ", ");
    ret.append(to_string(this->normal.y()) + ", ");
    ret.append(to_string(this->normal.z()));
    ret.append("], \n");
    ret.append(" \"coord\" : [");
    for (unsigned int i = 0 ; i < this->getVerticesSize() ; i++){
        ret.append(this->v_list[i]->toJSON());
        ret.append(",");
    }
    ret.append(this->v_list[0]->toJSON());
    ret.append("] }");
    return ret;
}

std::string Surface::toJSONWithTriangles() {
    string indent("\t");

    string ret;
    ret += "{";
    ret.append(" \n \"area\" : " + to_string(area) );
    ret.append(" \n, \"id\" : " + to_string(sf_id) );
    ret.append( "\n, \"triangles\" : [\n");
    for (int i = 0 ; i < (int)this->triangles.size() ; i++){
        ret += this->triangles[i]->toJSON(indent);
        if (i != this->triangles.size() - 1)
            ret += ",";
        ret += "\n";
    }
    ret.append( "]\n");
    ret.append( "}");

    return ret;
}


Vector_3 Surface::getSimpleNormal(){
    Vector_3 normal = Vector_3(0,0,0);
    for (int i = 0 ; i < (int) this->getVerticesSize() - 1 ; i += 2){
        int e_i = i + 2 >= (int) this->getVerticesSize()? 0 : i+2;
        normal = normal + CGALCalculation::getCrossProduct(v_list[i], v_list[i+1], v_list[e_i]);
    }
    return normal;
}


bool Surface::updateNormal(){
    if (this->getVerticesSize() <= 4){
        this->normal = getSimpleNormal();
    }

    this->normal = this->normal / sqrt(this->normal.squared_length());
    this->normal = this->normal * this->area * AREA_CONST;

    if (this->normal == CGAL::NULL_VECTOR){
        cout << "NULLVECTOR" << endl;
        assert(this->normal != CGAL::NULL_VECTOR);
    }

    return true;

}

bool Surface::isOpposite(Surface* sf){
    for (ll i = 0 ; i < (ll) sf->getVerticesSize() ; i++){
        if (this->v_list[0] == sf->v_list[i]){
            ll sf_index = i + 1;
            if (sf_index == (ll) sf->getVerticesSize()) sf_index = 0;
            ll this_index = v_list.size() - 1;
            while (this->v_list[this_index] == sf->v_list[sf_index]){
                this_index--; sf_index++;
                if (sf_index == (ll) sf->getVerticesSize()) sf_index = 0;
                if (this_index == 0 || sf_index == i) break;

            }
            if (this->v_list[this_index] != sf->v_list[sf_index])
                return false;
            else
                return true;

        }
    }
    return false;
}


bool Surface::isAdjacent(Surface* sf){
    return SurfacePairComputation::isShareVertex(this->v_list, sf->v_list);
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

bool Surface::compareLength(Surface* i, Surface* j) {
     return (i->getVerticesSize() > j->getVerticesSize());
}

bool Surface::compareArea(Surface* i, Surface* j) {
     return (i->area > j->area);
}


/**
*  Check that Surface is not valid. Straight Line or Point.
*/
bool Surface::isValid(){
    if (this->getVerticesSize() < 3) {
        cout << "The number of vertexes is "  << this->getVerticesSize() <<endl;
        return false;
    }

    return true;
}


Point_3 Surface::findLowestPoint(){
    Vertex* cent = SurfaceComputation::getCenterPoint(this);
    Plane_3 plane(VertexComputation::getCGALPoint(cent), this->normal);
    delete cent;

    double max_dist = -1.0;
    int max_index = 0;
    for (ull index= 0 ; index < this->getVerticesSize() ; index++){
        Point_3 p = VertexComputation::getCGALPoint(this->v_list[index]);
        if (plane.oriented_side(p) != CGAL::ON_POSITIVE_SIDE){
            double dist = CGAL::squared_distance(plane, p);
            if (dist > max_dist){
                max_dist = dist;
                max_index = index;
            }
        }
    }
    return VertexComputation::getCGALPoint(this->v_list[max_index]);
}

Plane_3 Surface::getPlaneWithLowest(){
    Point_3 point = findLowestPoint();
    return Plane_3(point, this->normal);
}

void Surface::removeVertexByIndex(int id){
    this->v_list.erase(this->v_list.begin() + id);
}

vector<Vertex *> Surface::getVerticesList() {
    return this->v_list;
}

Vertex *Surface::vertex(ull i) {
    return v_list[i];
}

void Surface::setVertices(std::vector<Vertex *> newVertices) {
    this->v_list = newVertices;
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
    this->v_list.erase(this->v_list.begin() + startIndex, this->v_list.begin() + endIndex);
}

void Surface::setVertex(int index, Vertex *vt) {
    this->v_list[index] = vt;
}

void Surface::insertVertex(int index, Vertex *vt) {
    this->v_list.insert(this->v_list.begin() + index, vt);
}

void Surface::setPlaneRef(Plane_3 pl) {
    this->planeRef = pl;
}
