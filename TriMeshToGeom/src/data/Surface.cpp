//
//  Surface.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 19..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "logic/check.hpp"
#include "data/Surface.hpp"
#include "logic/Triangulator.h"
#include "logic/CleanPolygonMaker.h"

#include "predefine.h"

#include <cstdlib>
#include <cmath>

using namespace std;

Surface::Surface(Surface* cp){
    this->v_list = cp->v_list;
    this->av_normal = cp->av_normal;
    for (int i = 0 ; i < 3 ; i++){
        this->max_coords[i] = cp->max_coords[i];
        this->min_coords[i] = cp->min_coords[i];
    }
    this->area = cp->area;
    this->tri_list = cp->tri_list;
}

void Surface::setZ(double value){
    for (ull i = 0 ; i < this->v_list.size() ; i++){
        v_list[i]->setZ(value);
    }
}

Surface::Surface(Triangle& pl){
    Vertex* v[3] = {pl.a, pl.b, pl.c};

    for (int i = 0 ; i < 3 ;i++){
        v_list.push_back(v[i]);
    }

    av_normal = pl.getNormal();
    area = pl.getArea();
    this->setMBB();
    this->tri_list.push_back(&pl);
}

void Surface::setMBB(){
    for (int i = 0 ; i < 3 ; i++)
    {
        this->max_coords[i] = -10000000.000;
        this->min_coords[i] = 10000000.00;
    }

    for (unsigned int i = 0 ; i < this->v_list.size() ; i++){
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
/**
 * if this Vertex(add_id) try to make hole, return true.
 */
bool Surface::isExistSameVertexInRange(ll si, ll ei, Vertex* add_id){
    for (ll i = si ; i != ei ; i++)
    {
        if (i == (int)this->v_list.size()){
            i = -1;
            continue;
        }
        if (v_list[i] == add_id)
        {
            return true;
        }
    }
    return false;
}


int Surface::getSegmentsNumber(ll si, ll ei){
    int num = 0;
    for (ll i = si ; ;){
        if (i == ei) break;
        if (num > (int)this->v_list.size()) {
            cout << "getSegmentsNumber Error" << endl;
            return -1;
        }
        num++;
        i++;
        if (i == (int)this->v_list.size()) i = 0;
    }
    return num;
}

bool Surface::checkDuplicate(Checker* ch){
    vector<Vertex*> sorted_v_list(this->v_list);

    sort(sorted_v_list.begin(), sorted_v_list.end(), Vertex::compare);
    for (ull i = 0 ; i < sorted_v_list.size() - 1; i++){
        if (sorted_v_list[i] == sorted_v_list[i+1]){
            cout << "Same Index" << endl;
            return true;
        }
        if (ch != NULL && ch->isSameVertex(sorted_v_list[i], sorted_v_list[i+1])){
            cout << "Same Coords" << endl;
            cout << sorted_v_list[i]->toJSON() << endl;
            cout << sorted_v_list[i+1]->toJSON() <<endl;
            return true;
        }
    }
    return false;
}


Point_3 Surface::getCenterPoint(){
    double x=0.0,y=0.0,z=0.0;
    x = this->min_coords[0] + this->max_coords[0];
    y = this->min_coords[1] + this->max_coords[1];
    z = this->min_coords[2] + this->max_coords[2];
    x = x/2;
    y = y/2;
    z = z/2;
    Point_3 p(x,y,z);
    return p;
}

Point_3 Surface::getCenterPointInFartest(){
    double sq_dist = -1;
    int si = 0, sj = 1;
    for (int i = 0 ; i < (int)this->v_list.size() - 1; i++){
        for (int j = 1; j < (int)this->v_list.size() ; j++){
            double dist = CGALCalculation::getSquaredDistance(this->v_list[i], this->v_list[j]);
            if (dist > sq_dist){
                sq_dist = dist;
                si = i;
                sj = j;
            }
        }
    }

    return CGAL::midpoint(this->v_list[si]->getCGALPoint(), this->v_list[sj]->getCGALPoint());
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
    for (unsigned int i = 0 ; i < this->v_list.size() ; i++){
        if (!this->v_list[i]->used){
            cout << "Wrong in TOJSONSTRING" << endl;
            exit(-1);
        }
        ret.append(this->v_list[i]->toJSON());
        ret.append(",");
    }
    ret.append(this->v_list[0]->toJSON());
    ret.append("] }");
    return ret;
}

Vector_3 Surface::getSimpleNormal(){
    Vector_3 normal = Vector_3(0,0,0);
    for (int i = 0 ; i < (int)this->v_list.size() - 1 ; i += 2){
        int e_i = i + 2 >= (int)this->v_list.size()? 0 : i+2;
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
    Plane_3 plane = Plane_3(this->v_list[0]->getCGALPoint(), CGALCalculation::normal_list18[type]);
    for (ull i = 0 ; i < this->v_list.size() ; i++){
        Point_3 p3 = this->v_list[i]->getCGALPoint();
        Point_2 point2d = plane.to_2d(p3);
        points.push_back(make_pair(point2d.x(), point2d.y()));
    }

    return points;
}

bool Surface::updateNormal(Checker* ch){
    if (this->v_list.size() <= 4){
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

void Surface::updateRectArea(){
    this->area = 0.0;
    for (int i = 0 ; i < (int)this->v_list.size() - 1 ; i += 2){
        int e_i = i + 2 >= (int)this->v_list.size()? 0 : i+2;
        Triangle tri(this->v_list[i], this->v_list[i+1], this->v_list[e_i]);
        area += tri.getArea();
    }
}

bool Surface::isOpposite(Surface* sf){
    for (ll i = 0 ; i < (ll)sf->v_list.size() ; i++){
        if (this->v_list[0] == sf->v_list[i]){
            ll sf_index = i + 1;
            if (sf_index == (ll)sf->v_list.size()) sf_index = 0;
            ll this_index = v_list.size() - 1;
            while (this->v_list[this_index] == sf->v_list[sf_index]){
                this_index--; sf_index++;
                if (sf_index == (ll)sf->v_list.size()) sf_index = 0;
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
    return CleanPolygonMaker::isShareVertex(this->v_list, sf->v_list);
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
     return (i->getLength() > j->getLength());
}

bool Surface::compareArea(Surface* i, Surface* j) {
     return (i->area > j->area);
}

void Surface::removeStraight(double degree){
    if (this->v_list.size() < 3) return;
    ll index = 1;
    Vertex* start_p = this->v_list[0];
    Vertex* check_p = this->v_list[index];

    int removed_count = 0;
    vector<Vertex*> new_v_list;
    do {
        ll next_index = index + 1;
        if (next_index == (ll)this->v_list.size()) next_index = 0;
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

void Surface::removeConsecutiveDuplication(Checker* ch){
    ull v_size = v_list.size();
    int removed_count = 0;
    for (ull i = 0 ; i < v_size - 1; i++){
        if (ch->isSameVertex(v_list[i] , v_list[i+1])){
            v_list.erase(v_list.begin() + i + 1);
            i--;
            v_size -= 1;
            removed_count += 1;
        }
    }

    if (removed_count) cout << removed_count << " are removed in duplication" << endl;
}

void Surface::removeHole(Checker* ch)
{
    int removed_count= 0;
    bool isChagned = false;
    do
    {
        isChagned = false;
        for (ull i = 0 ; i < v_list.size() - 1 ; i++)
        {
            for (ull j = i + 1; j < v_list.size() ; j ++)
            {
                if (ch->isSameVertex(v_list[i], v_list[j]))
                {
                    v_list.erase(v_list.begin() + i, v_list.begin() + j);
                    removed_count += j - i;
                    isChagned = true;
                    break;
                }
            }
            if (isChagned) break;
        }
    }
    while (isChagned);

    if (removed_count) cout << removed_count << " are removed in removeHole" << endl;
}

bool Surface::hasSameNormalwith(int axis){
    return CGALCalculation::getAngle(CGALCalculation::normal_list6[axis], this->av_normal) < 0.0001 ;
}

bool Surface::hasOppositeNormalwith(int axis){
    return CGALCalculation::getAngle(CGALCalculation::normal_list6[axis + 3], this->av_normal) < 0.0001 ;
}

/**
*  Check that Surface is not valid. Straight Line or Point.
*/
bool Surface::isValid(){
    if (this->v_list.size() < 3) {
        cout << "The number of vertexes is "  << this->v_list.size() <<endl;
        return false;
    }
    return true;
    /*
    bool isNOTcollinear = false;
    Point_3 start_p = this->v_list[0]->getCGALPoint();
    Point_3 end_p = this->v_list[1]->getCGALPoint();
    for (ll i = 1 ; i < (ll)this->v_list.size() - 1; i++){
        if (this->v_list[i] == nullptr){
            assert(this->v_list[i] != nullptr);
        }
        Point_3 mid_p(end_p.x(), end_p.y(), end_p.z());
        end_p = this->v_list[i+1]->getCGALPoint();
        if (CGAL::collinear(start_p, mid_p, end_p)){
            continue;
        }
        else{
            isNOTcollinear = true;
        }
    }
    return isNOTcollinear;
    */
}

void Surface::tagVerticesUsed(){
    for (ull i = 0 ; i < this->v_list.size() ; i++){
        this->v_list[i]->used = true;
    }
}

Point_3 Surface::findLowestPoint(){
    Plane_3 plane(getCenterPoint(), this->av_normal);

    double max_dist = -1.0;
    int max_index = 0;
    for (ull index= 0 ; index < this->v_list.size() ; index++){
        Point_3 p = this->v_list[index]->getCGALPoint();
        if (plane.oriented_side(p) != CGAL::ON_POSITIVE_SIDE){
            double dist = CGAL::squared_distance(plane, p);
            if (dist > max_dist){
                max_dist = dist;
                max_index = index;
            }
        }
    }
    return this->v_list[max_index]->getCGALPoint();
}

Plane_3 Surface::getPlaneWithLowest(){
    Point_3 point = findLowestPoint();
    return Plane_3(point, this->av_normal);
}

void Surface::makePlanar(Plane_3 plane){
    for (ull index = 0 ; index < this->v_list.size() ; index++ )
    {
        Point_3 point(this->v_list[index]->x(),this->v_list[index]->y(),this->v_list[index]->z());
        Point_3 projected = plane.projection(point);

        this->v_list[index]->setX(projected.x());
        this->v_list[index]->setY(projected.y());
        this->v_list[index]->setZ(projected.z());
    }
}

vector<Point_2> Surface::get2DPoints(Plane_3 plane){
    vector<Point_2> points;

    for (ull i = 0 ; i < this->v_list.size() ; i++){
        Point_3 p3 = this->v_list[i]->getCGALPoint();
        Point_2 point2d = plane.to_2d(p3);
        points.push_back(point2d);
    }

    return points;
}

void Surface::changeToRectangle(){
    Plane_3 plane = getPlaneWithLowest();
    vector<Point_2> points_2d = get2DPoints(plane);
    double max_x = INT_MIN, max_y = INT_MIN;
    double min_x = INT_MAX, min_y = INT_MAX;

    // MBR
    for (ull i = 0 ; i < points_2d.size() ; i++){
        double x = points_2d[i].x();
        double y = points_2d[i].y();
        if (max_y < y){
            max_y = y;
        }
        if (min_y > y){
            min_y = y;
        }
        if (max_x < x){
            max_x = x;
        }
        if (min_x > x){
            min_x = x;
        }
    }

    Point_2 min_point(min_x,min_y);
    Point_2 center_1(max_x, min_y);
    Point_2 max_point(max_x,max_y);
    Point_2 center_2(min_x, max_y);

    vector<Point_3> points_3d;
    points_3d.push_back(plane.to_3d(min_point));
    points_3d.push_back(plane.to_3d(center_1));
    points_3d.push_back(plane.to_3d(max_point));
    points_3d.push_back(plane.to_3d(center_2));

    this->v_list.clear();
    for (ull i = 0 ; i < points_3d.size() ; i++){
        this->v_list.push_back(new Vertex(points_3d[i].x(),points_3d[i].y(),points_3d[i].z()));
    }

    points_2d.clear();
    points_3d.clear();
}

Segment* Surface::makeSegmentLowerZ(Checker* ch){
    Vertex* ft, *ed;

    //Only For Rectangle
    if (this->v_list.size() != 4){
        assert(v_list.size() == 4);
    }

    for (ull i = 0 ; i < 2; i++){
        if (ch -> isSameZ(this->v_list[i], this->v_list[i+1])){
            if (this->v_list[i+2]->z() > this->v_list[i+1]->z()){
                ft = this->v_list[i];
                ed = this->v_list[i+1];
                break;
            }
            else{ // i,i+1 > i+2
                if (i == 0){
                    ft = this->v_list[2];
                    ed = this->v_list[3];
                }
                else{ //i == 1
                    ft = this->v_list[3];
                    ed = this->v_list[0];
                }
                break;
            }

        }
    }

    return new Segment(ft, ed);
}

Segment* Surface::makeSegmentUpperZ(Checker* ch){
    Vertex* ft, *ed;

    //Only For Rectangle
    if (this->v_list.size() != 4){
        assert(v_list.size() == 4);
    }

    for (ull i = 0 ; i < 2; i++){
        if (ch -> isSameZ(this->v_list[i], this->v_list[i+1])){
            if (this->v_list[i+2]->z() > this->v_list[i+1]->z()){
                if (i == 0){
                    ft = this->v_list[2];
                    ed = this->v_list[3];
                }
                else{
                    ft = this->v_list[3];
                    ed = this->v_list[0];
                }
                break;
            }
            else{
                ft = this->v_list[i];
                ed = this->v_list[i+1];
                break;
            }

        }
    }

    return new Segment(ft, ed);
}

void Surface::snapping(Surface* p_surface, double p_diff){
    vector<pair<ull,ull>> match_list;
    int match_num = 0;
    for (ull vj = 0 ; vj < p_surface->v_list.size() ; vj++){
        for (ull vi = 0 ; vi < this->v_list.size() ; vi++){
            if (this->v_list[vi]->index == p_surface->v_list[vj]->index){
                match_list.push_back(make_pair(vi, vj));
                match_num++;
                break;
            }
        }
    }
    if (match_num == 0) return;

    for (int i = 0 ; i < match_list.size() - 1; i++){
        ull next_first = match_list[i].first == 0? this->v_list.size() - 1 : match_list[i].first - 1;
        if (match_list[i].second + 1 != match_list[i+1].second){
            if (next_first != match_list[i+1].first){
                ull nn_first = next_first == 0 ? this->v_list.size() - 1 : next_first - 1;
                if (nn_first == match_list[i+1].first){
//                    removed_first.push_back(next_first);
//                    removed_second.push_back(match_list[i].second + 1);
                    this->v_list[next_first] = p_surface->v_list[match_list[i].second + 1];
                }
                else{
                    p_surface->v_list[match_list[i].second + 1] = p_surface->v_list[match_list[i].second];
                    //removed_second.push_back(match_list[i].second + 1);
                }
            }
            else{
                p_surface->v_list[match_list[i].second + 1] = p_surface->v_list[match_list[i].second];
                //removed_second.push_back(match_list[i].second + 1);
            }
        }
        else{
            if (next_first != match_list[i+1].first){
                ull nn_first = next_first == 0 ? this->v_list.size() - 1 : next_first - 1;
                if (nn_first == match_list[i+1].first){
                    //removed_first.push_back(next_first);
                    this->v_list[next_first] = this->v_list[match_list[i].first];
                }
            }
            else{
            }
        }
    }

}

void Surface::clipping(Surface* p_surface, Checker* ch){
    int num = 0;
    for (ull i = 0 ; i < this->v_list.size() ; i++){
        Vertex* vi = this->v_list[i];
        for (ull j = 0 ; j < p_surface->v_list.size() ; j++){
            Vertex* vj = p_surface->v_list[j];
            if (vi != vj && ch->isSameVertex(vi, vj)){
                num++;
                delete vi;
                this->v_list[i] = p_surface->v_list[j];
                break;
            }
        }
    }
}
