#include "features/Surface.hpp"
#include "logic/check.hpp"
#include "space_maker/OnlyWallSpaceMaker.h"
#include "features/SurfaceGraph.h"
#include "compute/SurfacesListComputation.h"
#include "cgal/Types.h"

class Segment{
public :
    Vertex* first;
    Vertex* second;

    Segment(){}
    Segment(Vertex* st, Vertex* ed){
        first = st;
        second = ed;
    }

    Vertex* operator[](int idx){
        if (idx == 0) return first;
        else if (idx == 1) return second;
        else assert("idx is 1 0r 0");
        return nullptr;
    }

    std::string toJSON(){
        std::string ret;
        ret += first->toJSON();
        ret += " --> ";
        ret += second->toJSON();
        return ret;
    }

    double getSquaredDistance(){
        return CGALCalculation::getSquaredDistance(this->first, this->second);
    }

    static double compareDistanceIncreased(Segment* s_i, Segment* s_j){
        return s_i->getSquaredDistance() < s_j->getSquaredDistance();
    }
    Segment_2 getCGALSegmentWithoutZ(){
        Point_2 p1(this->first->x(), this->first->y());
        Point_2 p2(this->second->x(), this->second->y());

        return Segment_2(p1, p2);
    }

    static bool isIntersect2D(Segment* seg1, Segment* seg2){
        return CGAL::do_intersect(seg1->getCGALSegmentWithoutZ(), seg2->getCGALSegmentWithoutZ());
    }

    static Point_2 getIntersection2D(Segment* seg1, Segment* seg2){
        Segment_2 seg1_cgal = seg1->getCGALSegmentWithoutZ();
        Segment_2 seg2_cgal = seg2->getCGALSegmentWithoutZ();
        CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type result = CGAL::intersection(seg1_cgal, seg2_cgal);
        Point_2* p = boost::get<Point_2 >(&*result);
        return *p;
    }
};


vector<Surface*> OnlyWallSpaceMaker::makeSimpleSurfaces(vector<Surface*> _surfacesList){
    SurfaceGraph* sg = new SurfaceGraph();
    sg->makeAdjacentGraph(_surfacesList);
    this->removeSurfacesNotConnectedFC(_surfacesList, sg);
    this->removeOppositeSurfaces(_surfacesList);
    this->makeSurfacesPlanarWithLowest(_surfacesList);
    this->makeWallRectangle(_surfacesList);
    this->makeClosedWall(_surfacesList);
    this->makeFloorAndCeiling(_surfacesList);

    return _surfacesList;
}

Surface* OnlyWallSpaceMaker::findFirstSurfaceSimilarWithAxis(vector<Surface*>& surfacesList, int axis){
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        Surface* sf = surfacesList[i];
        if (CGALCalculation::findNormalType6(sf->av_normal) == axis){
            return sf;
        }
    }
    assert(false);
}

//Floor And Ceiling
int OnlyWallSpaceMaker::removeSurfacesNotConnectedFC(vector<Surface*>& surfacesList, SurfaceGraph* surface_graph){
    sort(surfacesList.begin(), surfacesList.end(), Surface::compareArea);

    Surface* roof = OnlyWallSpaceMaker::findFirstSurfaceSimilarWithAxis(surfacesList, 5);
    Surface* floor = OnlyWallSpaceMaker::findFirstSurfaceSimilarWithAxis(surfacesList, 2);

    if (roof == NULL || floor == NULL) return -1;

    for (ull i = 0 ; i < surfacesList.size() ; ){
        Surface* sf = surfacesList[i];
        if ( sf == roof || sf == floor) {
            i++;
        }
        else if ( surface_graph->isNeighbor(sf->sf_id, roof->sf_id) &&
                  surface_graph->isNeighbor(sf->sf_id, floor->sf_id)){
            i++;
        }
        else{
            delete(surfacesList[i]);
            surfacesList.erase(surfacesList.begin() + i);
        }
    }
    return 0;
}


//TODO : not remove -> cut
int OnlyWallSpaceMaker::removeOppositeSurfaces(vector<Surface*>& surfacesList){
    bool changed = true;
    while (changed){
        changed = false;
        for (ull i = 0 ; i < surfacesList.size() ; i++){
            Surface* isf = surfacesList[i];
            for (ull j = i + 1; j < surfacesList.size() ; j++){
                Surface* jsf = surfacesList[j];
                if ( CGALCalculation::getAngle(isf->av_normal, jsf->av_normal) > 179.99){
                    if ( isf->isAdjacent(jsf) ){
                        surfacesList.erase(surfacesList.begin() + j);
                        surfacesList.erase(surfacesList.begin() + i);
                        changed = true;
                    }
                }
            }
        }
    }
    return 0;
}

int OnlyWallSpaceMaker::makeSurfacesPlanarWithLowest(vector<Surface*>& surfacesList){
    cout << "\n------------- make planar --------------\n" << endl;

    for (int axis = 2 ; axis >= 0  ; axis--){
        for (ull i = 0 ; i < surfacesList.size() ; i++){
            Surface* surface = surfacesList[i];
            if (surface->hasSameNormalwith(axis) || surface->hasOppositeNormalwith(axis))
            {
                Plane_3 plane = surface->getPlaneWithLowest();
                surface->makePlanar(plane);
            }
        }
    }

    return 0;
}



int OnlyWallSpaceMaker::makeWallRectangle(vector<Surface*>& surfacesList){
    cout << "\n------------- make Wall Rectangle --------------\n" << endl;
    for (int axis = 1 ; axis >= 0  ; axis--){
        for (ull i = 0 ; i < surfacesList.size() ; i++){
            Surface* surface = surfacesList[i];
            if (surface->hasSameNormalwith(axis) || surface->hasOppositeNormalwith(axis))
            {
                surface->changeToRectangle();
            }
        }
    }
    return 0;
}

Surface* OnlyWallSpaceMaker::makeNewSurface(Segment* seg, double base, double height){
    Surface* new_surface = new Surface();

    Vertex* vt1 = new Vertex(*seg->first);
    vt1->setZ(0);

    Vertex* vt2 = new Vertex(*seg->second);
    vt2->setZ(0);

    new_surface->v_list.push_back(seg->first);
    new_surface->v_list.push_back(seg->second);
    new_surface->v_list.push_back(vt2);
    new_surface->v_list.push_back(vt1);

    new_surface->updateRectArea();
    new_surface->updateNormal(this->checker);
    return new_surface;
}


int OnlyWallSpaceMaker::makeClosedWall(vector<Surface*>& surfacesList){
    cout << "-------- make Closed Wall ------"<<endl;
    vector<Surface*> walls;
    vector<Segment*> walls_2d;

    double base = 0;
    vector<vector<double>> v_minmax;
    v_minmax = SLC::getMBB(surfacesList);
    double height = v_minmax[1][2];

    this->getWallsAndRemoveInSurfacesList(surfacesList, walls);
    this->clippingSurfaces(walls);

    for (ull i = 0 ; i < walls.size() ; i++){
        walls_2d.push_back(makeSegmentUpperZ(walls[i], this->checker));
    }
    this->cutIntersection(walls_2d);

    assert (base < 0.0000001);

    vector<bool> checked(walls_2d.size(), false);
    vector<vector<int> > ordered_index;

    int check_num = 0;
    while ( check_num < (int)walls_2d.size() ){
        int line_index = 0;
        vector<int> ordered;
        for (; line_index < (int)walls_2d.size() ;){
            if (checked[line_index]) {
            }
            else if (ordered.size() == 0) {
                check_num++;
                checked[line_index] = true;
                ordered.push_back(line_index);
            }
            else{
                Segment* seg_front = walls_2d[ordered[0]];
                Segment* seg_end = walls_2d[ordered[ordered.size() - 1]];
                if ( seg_front -> first == walls_2d[line_index] -> second ){
                    ordered.insert(ordered.begin(), line_index);
                    check_num++;
                    checked[line_index] = true;
                    line_index = -1;
                }
                else if (seg_end->second == walls_2d[line_index]->first){ //end
                    ordered.push_back(line_index);
                    check_num++;
                    checked[line_index] = true;
                    line_index = -1;
                }
            }
            line_index++;
        }
        ordered_index.push_back(ordered);
    }

//    for (int i = 0 ; i < ordered_index.size() ; i ++){
//        for (int j = 0 ; j < ordered_index[i].size() ; j++){
//            cout << ordered_index[i][j] << " ";
//        }
//        cout << endl;
//    }

    vector<bool> checked_2(ordered_index.size(), false);
    bool can_connect = connectWall_dfs(0, ordered_index, walls_2d, checked_2);
    assert(can_connect);

    walls.clear();
    for (ull i = 0 ; i < walls_2d.size() ; i++){
        walls.push_back(makeNewSurface(walls_2d[i], base, height));
    }
    walls = this->clippingSurfaces(walls);

    for (ull i = 0 ; i < walls_2d.size() ; i++){
        surfacesList.push_back(walls[i]);
    }
    cout << "new Walls Size : "<< walls_2d.size() - check_num << endl;
    cout << "Walls Size : " << check_num << endl;

    walls.clear();
    walls_2d.clear();

    for (ull i = 0 ; i < ordered_index.size() ; i++){
        ordered_index[i].clear();
    }
    ordered_index.clear();

    return 0;
}

int OnlyWallSpaceMaker::makeFloorAndCeiling(vector<Surface*>& surfacesList){
    cout << "--------make Floor and Ceiling -------" << endl;
    Surface* ceil = new Surface();
    Surface* floor = new Surface();
    ceil->av_normal = CGALCalculation::normal_list6[5];
    floor->av_normal = CGALCalculation::normal_list6[2];

    vector<Segment*> walls_2d;
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        walls_2d.push_back(makeSegmentUpperZ(surfacesList[i], this->checker));
    }

    int line_index = 0;
    vector<bool> checked(walls_2d.size(), false);
    vector<int> ordered;
    for (; line_index < (int)walls_2d.size() ;){
        if (checked[line_index]) {
        }
        else if (ordered.size() == 0) {
            checked[line_index] = true;
            ordered.push_back(line_index);
        }
        else{
            Segment* seg_front = walls_2d[ordered[0]];
            Segment* seg_end = walls_2d[ordered[ordered.size() - 1]];
            if ( seg_front -> first == walls_2d[line_index] -> second ){
                ordered.insert(ordered.begin(), line_index);
                checked[line_index] = true;
                line_index = -1;
            }
            else if (seg_end->second == walls_2d[line_index]->first){ //end
                ordered.push_back(line_index);
                checked[line_index] = true;
                line_index = -1;
            }
        }
        line_index++;
    }

    assert(ordered.size() == walls_2d.size());

    for (int i = 0 ; i < (int)ordered.size() ; i++){
        int order = ordered[i];
        ceil->v_list.push_back(walls_2d[order]->first);
        floor->v_list.push_back(makeSegmentLowerZ(surfacesList[order], this->checker)->first);
    }

    reverse(floor->v_list.begin(), floor->v_list.end());

    surfacesList.push_back(floor);
    surfacesList.push_back(ceil);
    return 0;
}


int OnlyWallSpaceMaker::removeFloorAndCeiling(vector<Surface*>& surfacesList){
    cout << "\n------------- remove Object --------------\n" << endl;
    int axis = 2;
    int remove_count = 0;
    int remain_count = 0;
    for (ull i = 0 ; i < surfacesList.size() ; ){
        Surface* sf = surfacesList[i];
        if (sf->hasOppositeNormalwith(axis) || sf->hasSameNormalwith(axis)){
            delete sf;
            surfacesList.erase(surfacesList.begin() + i);
            remove_count++;
        }
        else{
            remain_count++;
            i++;
        }
    }
    cout << "remove : " << remove_count <<endl;
    cout << "remain : " << remain_count <<endl;

    return 0;
}

vector<Surface*> OnlyWallSpaceMaker::clippingSurfaces(vector<Surface*>& walls){
    assert (walls.size() > 0);
    for (ull i = 0 ; i < walls.size() - 1; i++){
        Surface* surface = walls[i];
        for (ull j = i + 1 ; j < walls.size(); j++){
            surface->clipping(walls[j], this->checker);
        }
        assert(surface->isValid());
    }
    return walls;
}

vector<Surface*> OnlyWallSpaceMaker::getWallsAndRemoveInSurfacesList(vector<Surface*>& surfacesList, vector<Surface*>& walls){
    int axis = 2;
    for (ull i = 0 ; i < surfacesList.size() ; ){
        Surface* surface = surfacesList[i];
        if (surface->hasSameNormalwith(axis) || surface->hasOppositeNormalwith(axis))
        {
            delete surface;
            surfacesList.erase(surfacesList.begin() + i);
        }
        else{
            walls.push_back(surface);
            surfacesList.erase(surfacesList.begin() + i);
        }
    }

    return walls;
}



bool OnlyWallSpaceMaker::connectWall_dfs(int index, vector<vector<int> >& ordered_index, vector<Segment*>& walls_2d, vector<bool>& checked){
    int num = 0 ;
    for (int i = 0 ; i < checked.size() ; i++){
        if (checked[i]) num ++;
    }
    if (num == (int)ordered_index.size()) return true;

    vector<int>& curr_order = ordered_index[index];
    int curr = curr_order[curr_order.size() - 1];
    Vertex* curr_end = walls_2d[curr]->second;

    vector<pair<int, Segment*>> candidates;
    for (int i = 0 ; i < (int)ordered_index.size() ; i++){
        if (checked[i]) continue;
        if (i == index) continue;
        vector<int>& next_order = ordered_index[i];
        int next = next_order[0];
        Vertex* next_first = walls_2d[next]->first;
        Segment* segment = new Segment(curr_end, next_first);
        if (isIntersectIn(segment, walls_2d)){
            //delete segment;
        }
        else{
            candidates.push_back(make_pair(i, segment));
        }
    }

    sort(candidates.begin(), candidates.end(), comparePairSegment);

    for (int i = 0 ; i < (int)candidates.size() ; i++){
        int next_index = candidates[i].first;
        Segment* next_seg = candidates[i].second;
        checked[next_index] = true;
        walls_2d.push_back(next_seg);
        if (connectWall_dfs(next_index, ordered_index, walls_2d, checked) ){
            return true;
        }
        checked[next_index] = false;
        walls_2d.erase(walls_2d.end());
    }

    return false;

}

vector<Segment*> OnlyWallSpaceMaker::cutIntersection(vector<Segment*>& walls_2d){
    for (ull i = 0 ; i < walls_2d.size() ; i++){
        for (ull j = i + 1 ; j < walls_2d.size() ; j++){
            if (Segment::isIntersect2D(walls_2d[i], walls_2d[j])){
                //is Clipped?
                if ((walls_2d[i]->first == walls_2d[j]->second) ||
                    (walls_2d[i]->second == walls_2d[j]->first)){

                }
                else{ // cut
                    Point_2 point = Segment::getIntersection2D(walls_2d[i], walls_2d[j]);
                    if (checker->isSameDouble(point.x(), walls_2d[i]->second->x())
                        && checker->isSameDouble(point.y(), walls_2d[i]->second->y())){
                        delete walls_2d[j]->first;
                        walls_2d[j]->first = walls_2d[i]->second;
                    }
                    else if (checker->isSameDouble(point.x(), walls_2d[j]->second->x())
                        && checker->isSameDouble(point.y(), walls_2d[j]->second->y())){
                        delete walls_2d[i]->first;
                        walls_2d[i]->first = walls_2d[j]->second;
                    }
                    else{//cross
                        Point_2 i_first(walls_2d[i]->first->x(), walls_2d[i]->first->y());
                        Point_2 j_second(walls_2d[j]->second->x(), walls_2d[j]->second->y());
                        Vertex* new_vertex = new Vertex(point.x(), point.y(), walls_2d[i]->first->z());
                        if (CGAL::left_turn(i_first, point, j_second)){
                            walls_2d[i]->second = new_vertex;
                            walls_2d[j]->first = new_vertex;
                        }
                        else{
                            walls_2d[j]->second = new_vertex;
                            walls_2d[i]->first = new_vertex;
                        }
                    }
                }
            }
        }
    }
    return walls_2d;
}



bool OnlyWallSpaceMaker::isIntersectIn(Segment* segment, vector<Segment*>& walls_2d){
    for (ull i = 0 ; i < walls_2d.size() ; i++){
        Segment* line_seg = walls_2d[i];
        if (segment->first == line_seg->second){
        }
        else if (segment->second == line_seg->first){
        }
        else{
            if (Segment::isIntersect2D(segment, line_seg) ){
                return true;
            }
        }
    }
    return false;
}

bool OnlyWallSpaceMaker::comparePairSegment(pair<int,Segment*>& a, pair<int,Segment*>& b){
    return a.second->getSquaredDistance() < b.second->getSquaredDistance();
}

Segment* OnlyWallSpaceMaker::makeSegmentLowerZ(Surface* sf, Checker* ch){
    Vertex* ft, *ed;

    //Only For Rectangle
    if (sf->v_list.size() != 4){
        assert(sf->v_list.size() == 4);
    }

    for (ull i = 0 ; i < 2; i++){
        if (ch -> isSameZ(sf->v_list[i], sf->v_list[i+1])){
            if (sf->v_list[i+2]->z() > sf->v_list[i+1]->z()){
                ft = sf->v_list[i];
                ed = sf->v_list[i+1];
                break;
            }
            else{ // i,i+1 > i+2
                if (i == 0){
                    ft = sf->v_list[2];
                    ed = sf->v_list[3];
                }
                else{ //i == 1
                    ft = sf->v_list[3];
                    ed = sf->v_list[0];
                }
                break;
            }

        }
    }

    return new Segment(ft, ed);
}

Segment* OnlyWallSpaceMaker::makeSegmentUpperZ(Surface* sf, Checker* ch){
    Vertex* ft, *ed;

    //Only For Rectangle
    if (sf->v_list.size() != 4){
        assert(sf->v_list.size() == 4);
    }

    for (ull i = 0 ; i < 2; i++){
        if (ch -> isSameZ(sf->v_list[i], sf->v_list[i+1])){
            if (sf->v_list[i+2]->z() > sf->v_list[i+1]->z()){
                if (i == 0){
                    ft = sf->v_list[2];
                    ed = sf->v_list[3];
                }
                else{
                    ft = sf->v_list[3];
                    ed = sf->v_list[0];
                }
                break;
            }
            else{
                ft = sf->v_list[i];
                ed = sf->v_list[i+1];
                break;
            }

        }
    }

    return new Segment(ft, ed);
}