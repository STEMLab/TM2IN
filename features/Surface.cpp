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
    this->av_normal = cp->av_normal;
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

    av_normal = pl.getNormal();
    area = pl.getArea();
    this->updateMBB();
    this->triangles.push_back(&pl);
}

void Surface::setZ(double value){
    for (ull i = 0 ; i < this->sizeOfVertices() ; i++){
        v_list[i]->setZ(value);
    }
}

void Surface::updateMBB(){
    for (int i = 0 ; i < 3 ; i++)
    {
        this->max_coords[i] = -10000000.000;
        this->min_coords[i] = 10000000.00;
    }

    for (unsigned int i = 0 ; i < this->sizeOfVertices() ; i++){
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
    if (si >= this->sizeOfVertices() || ei >= sizeOfVertices()){
        cerr << "getSegmentsNumber Error" << endl;
        return -1;
    }

    if (ei >= si){
        return ei - si;
    }
    else{
        return this->sizeOfVertices() - si + ei;
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
    string ret;
    ret.append("{");
    ret.append(" \n \"area\" : " + to_string(area) );
    ret.append(" \n, \"id\" : " + to_string(sf_id) );
    ret.append(" \n, \"normal\" : [");
    ret.append(to_string(this->av_normal.x()) + ", ");
    ret.append(to_string(this->av_normal.y()) + ", ");
    ret.append(to_string(this->av_normal.z()));
    ret.append("], \n");
    ret.append(" \"coord\" : [");
    for (unsigned int i = 0 ; i < this->sizeOfVertices() ; i++){
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
    for (int i = 0 ; i < (int)this->sizeOfVertices() - 1 ; i += 2){
        int e_i = i + 2 >= (int)this->sizeOfVertices()? 0 : i+2;
        normal = normal + CGALCalculation::getCrossProduct(v_list[i], v_list[i+1], v_list[e_i]);
    }
    return normal;
}

vector<pair<double, double>> Surface::project_to_Plane18(){
    vector<pair<double, double>> points;
    int type = CGALCalculation::findNormalType18(this->av_normal);
    if (this->av_normal == CGAL::NULL_VECTOR){
        exit(-1);
    }
    Plane_3 plane = Plane_3(VertexComputation::getCGALPoint(this->v_list[0]), CGALCalculation::normal_list18[type]);
    for (ull i = 0 ; i < this->sizeOfVertices() ; i++){
        Point_3 p3 = VertexComputation::getCGALPoint(this->v_list[i]);
        Point_2 point2d = plane.to_2d(p3);
        points.push_back(make_pair(point2d.x(), point2d.y()));
    }

    return points;
}

bool Surface::updateNormal(){
    if (this->sizeOfVertices() <= 4){
        this->av_normal = getSimpleNormal();
    }
    this->av_normal = CGALCalculation::normal_list18[CGALCalculation::findNormalType18(this->av_normal)];
    this->av_normal = this->av_normal / sqrt(this->av_normal.squared_length());
    this->av_normal = this->av_normal * this->area * AREA_CONST;
//
//    else{
//        vector<pair<double, double>> pointsInPlane = to2DPoints();
//        vector<vector<int>> indexes = Triangulator::triangulate2D(pointsInPlane, true);
//
//        if (indexes.size() == 0)
//        {
//            int randomType = rand() % 26 + 1;
//            cout << "Can not Triangulation " << randomType << endl;
//            this->av_normal = CGALCalculation::normal_list27[randomType];
//            return updateNormal(ch);
//        }
//
//        Vector_3 normal = Vector_3(0,0,0);
//        for (int i = 0 ; i < (int)indexes.size() ; i++)
//        {
//            Vector_3 new_normal = CGALCalculation::getUnitNormal(v_list[indexes[i][0]], v_list[indexes[i][1]], v_list[indexes[i][2]]);
//            normal = normal + new_normal;
//            cout << indexes[i][0] << " " << indexes[i][1] << " " << indexes[i][2] << endl;
//        }
//        cout << toJSONString() << endl;
//
//        indexes.clear();
//        pointsInPlane.clear();
//
//        int type = CGALCalculation::findNormalType27(normal);
//        this->av_normal = CGALCalculation::normal_list27[type];
//        this->av_normal = this->av_normal * this->sq_area * AREA_CONST;
//
//    }

    if (this->av_normal == CGAL::NULL_VECTOR){
        cout << "NULLVECTOR" << endl;
        assert(this->av_normal != CGAL::NULL_VECTOR);
    }

    return true;

}

bool Surface::isOpposite(Surface* sf){
    for (ll i = 0 ; i < (ll)sf->sizeOfVertices() ; i++){
        if (this->v_list[0] == sf->v_list[i]){
            ll sf_index = i + 1;
            if (sf_index == (ll)sf->sizeOfVertices()) sf_index = 0;
            ll this_index = v_list.size() - 1;
            while (this->v_list[this_index] == sf->v_list[sf_index]){
                this_index--; sf_index++;
                if (sf_index == (ll)sf->sizeOfVertices()) sf_index = 0;
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
     return (i->sizeOfVertices() > j->sizeOfVertices());
}

bool Surface::compareArea(Surface* i, Surface* j) {
     return (i->area > j->area);
}

void Surface::removeStraight(double degree){
    if (this->sizeOfVertices() < 3) return;

    vector<Vertex*> new_v_list;// = this->v_list;

    ll index = 1;
    Vertex* start_p = this->v_list[0];
    Vertex* check_p = this->v_list[index];
    int removed_count = 0;

    do {
        ll next_index = index + 1;
        if (next_index == (ll)this->sizeOfVertices()) next_index = 0;
        Vertex* end_p = this->v_list[next_index];
        if (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, degree)){
            removed_count++;
        }
        else{
            new_v_list.push_back(this->v_list[index]);
            start_p = this->v_list[index];
        }
        index = next_index;
        check_p = this->v_list[index];
    } while (index != 1);

    for (ull i = 1 ; i < new_v_list.size() - 1; ){
        Vertex* start_p = new_v_list[i-1];
        ull second = i;
        Vertex* check_p = new_v_list[second];
        ull third = i+1;
        Vertex* end_p = new_v_list[third];
        if (CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, degree)
           || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, degree) ){
            new_v_list.erase(new_v_list.begin() + i);
        }
        else if(CGALCalculation::isAngleLowerThan(start_p, check_p, end_p, -degree)
           || CGALCalculation::isAngleLowerThan(end_p, check_p, start_p, -degree) ){
            new_v_list.erase(new_v_list.begin() + i);
        }
        else{
            i++;
        }
    }

    this->v_list.clear();
    this->v_list = new_v_list;

    if (removed_count) cout << removed_count << " are removed in straight" << endl;
}

void Surface::removeConsecutiveDuplication(){
    ull v_size = v_list.size();
    int removed_count = 0;
    for (ull i = 0 ; i < v_size - 1; i++){
        if (Checker::isSameVertex(v_list[i] , v_list[i+1])){
            v_list.erase(v_list.begin() + i + 1);
            i--;
            v_size -= 1;
            removed_count += 1;
        }
    }

    if (removed_count) cout << removed_count << " are removed in duplication" << endl;
}


/**
*  Check that Surface is not valid. Straight Line or Point.
*/
bool Surface::isValid(){
    if (this->sizeOfVertices() < 3) {
        cout << "The number of vertexes is "  << this->sizeOfVertices() <<endl;
        return false;
    }

    return true;
}


Point_3 Surface::findLowestPoint(){
    Vertex* cent = SurfaceComputation::getCenterPoint(this);
    Plane_3 plane(VertexComputation::getCGALPoint(cent), this->av_normal);
    delete cent;

    double max_dist = -1.0;
    int max_index = 0;
    for (ull index= 0 ; index < this->sizeOfVertices() ; index++){
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
    return Plane_3(point, this->av_normal);
}

vector<Point_2> Surface::get2DPoints(Plane_3 plane){
    vector<Point_2> points;

    for (ull i = 0 ; i < this->sizeOfVertices() ; i++){
        Point_3 p3 = VertexComputation::getCGALPoint(this->v_list[i]);
        Point_2 point2d = plane.to_2d(p3);
        points.push_back(point2d);
    }

    return points;
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

std::vector<HalfEdge *> Surface::getboundaryEdgesList() {
    this->boundaryEdges = VertexListComputation::makeHalfEdgesList(this->getVerticesList());
    return boundaryEdges;
}

void Surface::clearTriangleList() {
    this->triangles.clear();
}