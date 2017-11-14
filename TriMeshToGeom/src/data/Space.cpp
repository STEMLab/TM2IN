#include "data/Space.h"

#include <cstdio>
#include <queue>
#include <climits>
#include <algorithm>
#include <cmath>


Space::Space(string pname, Checker* check)
{
    checker = check;
    name = pname;
}

Space::~Space()
{
    //dtor
}

void Space::pushTriangle(Triangle tri){
    this->triangles.push_back(tri);
}

int Space::makeSurfacesGreedy(double degree){
    int combined_count = 0;
    vector<Triangle*> p_triangles;
    ull size = this->triangles.size();
    for (ull i = 0 ; i < size; i++){
        p_triangles.push_back(&this->triangles[i]);
    }

    bool* checked = (bool*)malloc(sizeof(bool) * size);
    std::fill(checked, checked + size, false);

    vector<Surface*> c_list = makeSurfacesInList(p_triangles, checked, combined_count, degree);
    this->surfacesList.insert(this->surfacesList.end(), c_list.begin(), c_list.end());

    free(checked);

    this->triangles.clear();

    cout << "\ndone make Surfaces" << endl;
    return 0;
}

int Space::makeSurfacesNotJoin(){
    vector<Triangle*> p_triangles;
    ull size = this->triangles.size();
    for (ull i = 0 ; i < size; i++){
        p_triangles.push_back(&this->triangles[i]);
    }

    vector<Surface*> c_list;
    for (ull index = 0 ; index < size ; index++){
        Surface* newcp = new Surface(p_triangles[index]);
        c_list.push_back(newcp);
    }
    this->surfacesList.insert(this->surfacesList.end(), c_list.begin(), c_list.end());
    this->triangles.clear();

    cout << "\ndone make Surfaces" << endl;
    return 0;
}

void Space::printProcess(ull index, ull size){
    cout << "\r==========" << (int)((double)index/(double)size * 100) <<"% ========";
}


vector<Surface*> Space::makeSurfacesInList(vector<Triangle*>& tri_list, bool* checked, int& combined_count, double degree)
{
    vector<Surface*> result_list;
    ull size = tri_list.size();
    cout << "\n number : " << size << endl;

    for (ull index = 0 ; index < size; index++)
    {
        this->printProcess(combined_count, this->triangles.size());
        if (checked[index])
        {
            continue;
        }
        checked[index] = true ;
        Surface* newcp = new Surface(tri_list[index]);

        ll count = -1 ;
        while (count != 0)
        {
            newcp = attachTriangle(tri_list, newcp, checked, count, degree);
            if (newcp == NULL) break;
            if (combined_count % 250 == 1 )
            {
                cout << "\n------------ " << index << " -----------  size : " << size << endl;
                this->printProcess(combined_count, this->triangles.size());
            }
            combined_count += count;
        }
        if (newcp != NULL) result_list.push_back(newcp);
    }
    return result_list;
}


Surface* Space::attachTriangle(vector<Triangle*> tri_list, Surface* cp, bool* checked, ll& count, double degree)
{
    count = 0;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = 0 ; id < tri_list.size() ; id++)
    {
        if (!checked[id])
        {
            //if (cp->attachTriangle(tri_list[id], checker))
            if (TriangleAttacher::attach(cp, tri_list[id], checker, degree))
            {
                checked[id] = true;
                count++;
            }
        }
    }
    return cp;
}

int Space::checkTriangles(){
    cout << "check Triangles" << endl;
    ull size = this->triangles.size();
    for (ull index = 0 ; index < size -1; index++)
    {
        for (ull j = index + 1 ; j < size; j++)
        {
            if (triangles[index].isOpposite(triangles[j])){
                return -1;
            }
        }
    }
    return 0;
}

int Space::checkOpposite()
{
    for (unsigned int i = 0 ; i < this->surfacesList.size() - 1; i++)
    {
        for (unsigned int j = i + 1 ; j < this->surfacesList.size() ; j++)
        {
            if (this->surfacesList[i]->isOpposite(this->surfacesList[j]) )
            {
                return -1;
            }
        }
    }
    return 0;
}

int Space::combineSurfaceByArea(double degree){
    cout << "Combine Surfaces By area" << endl;

    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    ull p_size = this->surfacesList.size();
    bool* checked = (bool*)malloc(sizeof(bool) * p_size);
    std::fill(checked, checked + p_size, false);

    vector<Surface*> new_poly_list;
    int combined_count = 0;
    for (ull i = 0 ; i < this->surfacesList.size() ; i++)
    {
        if (checked[i]) continue;
        checked[i] = true;

        ll count = -1;
        Surface* newcp = new Surface(this->surfacesList[i]);
        while(count != 0){
            newcp = attachSurfacesByArea(newcp, i+1, checked, count, degree);
            if (newcp == NULL) break;
            this->printProcess(combined_count, 1);
            combined_count += count;
        }
        if (newcp != NULL) new_poly_list.push_back(newcp);
    }

    freeSurfaces();
    this->surfacesList = new_poly_list;
    return 0;
}

int Space::combineSurface(double degree){
    cout << "Combine Surfaces" << endl;

    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareLength);
    ull p_size = this->surfacesList.size();
    bool* checked = (bool*)malloc(sizeof(bool) * p_size);
    std::fill(checked, checked + p_size, false);

    vector<Surface*> new_poly_list;
    int combined_count = 0;
    for (ull i = 0 ; i < this->surfacesList.size() ; i++)
    {
        if (checked[i]) continue;
        checked[i] = true;

        ll count = -1;
        Surface* newcp = new Surface(this->surfacesList[i]);
        while(count != 0){
            newcp = attachSurfaces(newcp, i+1, checked, count, degree);
            if (newcp == NULL) break;
            this->printProcess(combined_count, 1);
            combined_count += count;
        }
        if (newcp != NULL) new_poly_list.push_back(newcp);
    }

    freeSurfaces();
    this->surfacesList = new_poly_list;
    return 0;
}

Surface* Space::attachSurfaces(Surface* cp, ull start, bool* checked, ll& count, double degree)
{
    count = 0;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = start ; id < this->surfacesList.size() ; id++)
    {
        if (!checked[id])
        {
            if (CleanPolygonMaker::combine(cp, this->surfacesList[id], checker, degree) == 0)
            {
                checked[id] = true;
                count++;
            }
        }
    }
    return cp;
}

Surface* Space::attachSurfacesByArea(Surface* cp, ull start, bool* checked, ll& count, double degree)
{
    count = 0;
    double cp_area = cp->area;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = start ; id < this->surfacesList.size() ; id++)
    {
        double id_area = this->surfacesList[id]->area;
        if (!checked[id] && (id_area < 0.00001 || id_area * 1000 < cp_area))
        {
            if (CleanPolygonMaker::combine(cp, this->surfacesList[id], checker, degree) == 0)
            {
                checked[id] = true;
                count++;
            }
        }
    }
    return cp;
}

int Space::updateNormal(){
    clock_t time_begin = clock();

    for (ull i = 0 ; i < (int)this->surfacesList.size() ; i++)
    {
        Surface* surface = this->surfacesList[i];
        if (!surface->updateNormal(this->checker))
        {
            cout << surface->toJSONString() <<endl;
            cout << "Cannot make Normal" << endl;
            exit(-1);
        }

    }

    clock_t time_end = clock();
    double elapsed_secs = double(time_end - time_begin) / CLOCKS_PER_SEC;
    cout << "update Normal time : " <<elapsed_secs << endl;
    return 0;
}

void Space::tagID(){
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareLength);
    for (ull i = 0 ; i < (ull)this->surfacesList.size() ; i++)
    {
        this->surfacesList[i]->sf_id = i;
    }
}

int Space::simplifySegment(){
    cout << "\n------------simplifySegment------------\n" << endl;
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareLength);
    ull p_size = this->surfacesList.size();

    for (ull i = 0 ; i < p_size - 1; i++)
    {
        printProcess(i, p_size);
        for (ull j = i + 1; j < p_size ; j++)
        {
            this->surfacesList[i]->updateNormal(checker);
            this->surfacesList[j]->updateNormal(checker);
            int loop_count = 0;
            bool again = false;
            while (CleanPolygonMaker::simplifyLineSegment(this->surfacesList[i], this->surfacesList[j], again) == 0)
            {
                again = true;
                loop_count++;
                if (loop_count > (int)this->surfacesList[j]->v_list.size()){
                    cout << "Infinite loop in Simplification" << endl;
                    exit(-1);
                }
                //loop
            }
        }
    }
    return 0;
}

int Space::handleDefect(){
    cout << "\n------------- handle Defect --------------\n" << endl;
    //sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    for (vector<Surface*>::size_type i = 0 ; i < this->surfacesList.size(); )
    {
        Surface* surface = this->surfacesList[i];
        surface->removeStraight(this->checker);
        surface->removeConsecutiveDuplication(this->checker);
        surface->setMBB();
        if (surface->checkDuplicate(this->checker)){
            surface->removeHole(this->checker);
        }

        if (surface->isValid()){
            i++;
        }
        else{
            delete surface;
            this->surfacesList.erase(this->surfacesList.begin() + i);
            cout << "Erase unvalid surface" << endl;
        }
    }
    return 0;
}




int Space::match00(){
    cout << "\n------------- match00 --------------\n" << endl;

    updateMBB();
    double diff[3];
    for (int i = 0 ; i < 3 ; i++){
        diff[i] = -this->min_coords[i];
    }

    for (ull i = 0 ; i < this->surfacesList.size() ; i++)
    {
        if (this->surfacesList[i]->isValid())
        {
            this->surfacesList[i]->translate(diff);
        }
        else{
            //this->polygon_list.erase(this->polygon_list.begin() + i);
            cout << this->surfacesList[i]->toJSONString() <<endl;
            cout << "un-valid surface in match00" << endl;
            exit(-1);
        }

    }

    for (ull i = 0 ; i < this->p_vertexList->size() ; i++){
        this->p_vertexList->at(i)->translate(diff);
    }

    return 0;
}

void Space::updateMBB(){
    for (int i = 0 ; i < 3 ; i++)
    {
        this->max_coords[i] = -1000000000;
        this->min_coords[i] = 1000000000;
    }

    for (ull i = 0 ; i < this->surfacesList.size() ; i++){
        Surface* cp = this->surfacesList[i];
        cp->setMBB();
        for (int j = 0 ; j < 3 ; j++){
            this->max_coords[j] = max(this->max_coords[j], cp->max_coords[j]);
            if (this->min_coords[j] > cp->min_coords[j]){
                this->min_coords[j] = cp->min_coords[j];
            }
        }
    }
}


int Space::remainOnlyUsingVertices(){
    for (ull i = 0 ; i < this->p_vertexList->size() ; i++){
        (*this->p_vertexList)[i]->used = false;
    }
    for (ull i = 0 ; i < this->surfacesList.size() ; i++){
        this->surfacesList[i]->tagVerticesUsed();
    }

    ull removed_count = 0;
    for (ull i = 0 ; i < this->p_vertexList->size() ;){
        if (this->p_vertexList->at(i)->used){
            this->p_vertexList->at(i)->index = i;
            i++;
        }
        else{
            delete(this->p_vertexList->at(i));
            this->p_vertexList->erase(this->p_vertexList->begin() + i);
            removed_count++;
        }
    }

    cout << "removed vertices : " << removed_count << endl;
    cout << "remained vertices : " << this->p_vertexList->size() << endl;;
    return 0;
}

Surface* Space::findFirstSurfaceSimilarWithAxis(int axis){
    for (ull i = 0 ; i < this->surfacesList.size() ; i++){
        Surface* sf = this->surfacesList[i];
        if (CGALCalculation::findNormalType6(sf->av_normal) == axis){
            return sf;
        }
    }
    assert(false);
}

Surface* Space::findFirstSurfaceWithAxis(int axis){
    for (ull i = 0 ; i < this->surfacesList.size() ; i++){
        Surface* sf = this->surfacesList[i];
        if (sf->hasSameNormalwith(axis)){
            return sf;
        }
    }
    assert(false);
}

//Floor And Ceiling
int Space::removeSurfacesNotConnectedFC(){
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    vector<bool> fixed_vertices(this->p_vertexList->size(), false);

    Surface* roof = findFirstSurfaceWithAxis(5);
    Surface* floor = findFirstSurfaceWithAxis(2);

    if (roof == NULL || floor == NULL) return -1;

    for (ull i = 0 ; i < this->surfacesList.size() ; ){
        Surface* sf = this->surfacesList[i];
        if ( sf == roof || sf == floor) {
            i++;
        }
        else if ( this->surface_graph->isNeighbor(sf->sf_id, roof->sf_id) &&
                  this->surface_graph->isNeighbor(sf->sf_id, floor->sf_id)){
            i++;
        }
        else{
            delete(this->surfacesList[i]);
            this->surfacesList.erase(this->surfacesList.begin() + i);
        }
    }
    return 0;
}


//TODO : not remove -> cut
int Space::removeOppositeSurfaces(){
    bool changed = true;
    while (changed){
        changed = false;
        for (ull i = 0 ; i < this->surfacesList.size() ; i++){
            Surface* isf = this->surfacesList[i];
            for (ull j = i + 1; j < this->surfacesList.size() ; j++){
                Surface* jsf = this->surfacesList[j];
                if ( CGALCalculation::getAngle(isf->av_normal, jsf->av_normal) > 179.99){
                    if ( isf->isAdjacent(jsf) ){
                        this->surfacesList.erase(this->surfacesList.begin() + j);
                        this->surfacesList.erase(this->surfacesList.begin() + i);
                        changed = true;
                    }
                }
            }
        }
    }
    return 0;
}

int Space::makeSurfacesPlanarWithLowest(){
    cout << "\n------------- make planar --------------\n" << endl;

    for (int axis = 2 ; axis >= 0  ; axis--){
        for (ull i = 0 ; i < this->surfacesList.size() ; i++){
            Surface* surface = this->surfacesList[i];
            if (surface->hasSameNormalwith(axis) || surface->hasOppositeNormalwith(axis))
            {
                Plane_3 plane = surface->getPlaneWithLowest();
                surface->makePlanar(plane);
            }
        }
    }

    return 0;
}


int Space::makeGraph(){
    cout << "\n------------- Graph --------------\n" << endl;
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    this->tagID();

    this->surface_graph = new SurfaceGraph();
    surface_graph->makeAdjacentGraph(this->surfacesList);

    surface_graph->print_bfs();
    return 0;
}

int Space::makeWallRectangle(){
    cout << "\n------------- make Wall Rectangle --------------\n" << endl;
    for (int axis = 1 ; axis >= 0  ; axis--){
        for (ull i = 0 ; i < this->surfacesList.size() ; i++){
            Surface* surface = this->surfacesList[i];
            if (surface->hasSameNormalwith(axis) || surface->hasOppositeNormalwith(axis))
            {
                surface->changeToRectangle();
            }
        }
    }
    return 0;
}

Surface* Space::makeNewSurface(Segment* seg, double base, double height){
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

vector<Surface*> Space::clippingSurfaces(vector<Surface*>& walls){
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

vector<Surface*> Space::getWallsAndRemoveInSurfacesList(vector<Surface*>& walls){
    int axis = 2;
    for (ull i = 0 ; i < this->surfacesList.size() ; ){
        Surface* surface = this->surfacesList[i];
        if (surface->hasSameNormalwith(axis) || surface->hasOppositeNormalwith(axis))
        {
            delete surface;
            this->surfacesList.erase(this->surfacesList.begin() + i);
        }
        else{
            walls.push_back(surface);
            this->surfacesList.erase(this->surfacesList.begin() + i);
        }
    }

    return walls;
}

int Space::countTheNumberOfVertex(vector<Segment*>& walls_2d){
    vector<Vertex*> vertex_list;
    for (ull i = 0 ; i < walls_2d.size(); i++){
        ull j;
        for (j = 0 ; j < vertex_list.size(); j++){
            if (vertex_list[j] == walls_2d[i]->first) {
                break;
            }
        }
        if (j == vertex_list.size() ) vertex_list.push_back(walls_2d[i]->first);

        for (j = 0 ; j < vertex_list.size(); j++){
            if (vertex_list[j] == walls_2d[i]->second) {
                break;
            }
        }
        if (j == vertex_list.size() ) vertex_list.push_back(walls_2d[i]->second);
    }
    int number_vertex = vertex_list.size();
    vertex_list.clear();
    return number_vertex;
}

bool Space::isIntersectIn(Segment* segment, vector<Segment*>& walls_2d){
    for (ull i = 0 ; i < walls_2d.size() ; i++){
        Segment* line_seg = walls_2d[i];
        if (segment->first == line_seg->second){
        }
        else if (segment->second == line_seg->first){
        }
        else{
            if (CGALCalculation::isIntersect2D(segment, line_seg) ){
                return true;
            }
        }
    }
    return false;
}

bool Space::comparePairSegment(pair<int,Segment*>& a, pair<int,Segment*>& b){
    return a.second->getSquaredDistance() < b.second->getSquaredDistance();
}

bool Space::connectWall_dfs(int index, vector<vector<int> >& ordered_index, vector<Segment*>& walls_2d, vector<bool>& checked){
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

vector<Segment*> Space::cutIntersection(vector<Segment*>& walls_2d){
    for (ull i = 0 ; i < walls_2d.size() ; i++){
        for (ull j = i + 1 ; j < walls_2d.size() ; j++){
            if (CGALCalculation::isIntersect2D(walls_2d[i], walls_2d[j])){
                //is Clipped?
                if ((walls_2d[i]->first == walls_2d[j]->second) ||
                    (walls_2d[i]->second == walls_2d[j]->first)){

                }
                else{ // cut
                    Point_2 point = CGALCalculation::getIntersection2D(walls_2d[i], walls_2d[j]);
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

int Space::makeClosedWall(){
    cout << "-------- make Closed Wall ------"<<endl;
    vector<Surface*> walls;
    vector<Segment*> walls_2d;

    this->updateMBB();
    walls = this->getWallsAndRemoveInSurfacesList(walls);
    walls = this->clippingSurfaces(walls);

    for (ull i = 0 ; i < walls.size() ; i++){
        walls_2d.push_back(walls[i]->makeSegmentUpperZ(this->checker));
    }
    walls_2d = this->cutIntersection(walls_2d);

    double base = 0;
    double height = this->max_coords[2];

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
        this->surfacesList.push_back(walls[i]);
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

int Space::makeFloorAndCeiling(){
    cout << "--------make Floor and Ceiling -------" << endl;
    Surface* ceil = new Surface();
    Surface* floor = new Surface();
    ceil->av_normal = CGALCalculation::normal_list6[5];
    floor->av_normal = CGALCalculation::normal_list6[2];

    vector<Segment*> walls_2d;
    for (ull i = 0 ; i < this->surfacesList.size() ; i++){
        walls_2d.push_back(surfacesList[i]->makeSegmentUpperZ(this->checker));
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
        floor->v_list.push_back(surfacesList[order]->makeSegmentLowerZ(this->checker)->first);
    }

    reverse(floor->v_list.begin(), floor->v_list.end());

    this->surfacesList.push_back(floor);
    this->surfacesList.push_back(ceil);
    return 0;
}

void Space::getVertexList(vector<Vertex*>& vt_list){
    cout << "get Vertex List" << endl;
    vector<Vertex*> new_vertices;
    vt_list = new_vertices;
}

void Space::freeSurfaces(){
    for (ull i = 0 ; i < this->surfacesList.size() ; i++)
    {
        delete(this->surfacesList[i]);
    }
    this->surfacesList.clear();
}




int Space::removeFloorAndCeiling(){
    cout << "\n------------- remove Object --------------\n" << endl;
    int axis = 2;
    int remove_count = 0;
    int remain_count = 0;
    for (ull i = 0 ; i < this->surfacesList.size() ; ){
        Surface* sf = this->surfacesList[i];
        if (sf->hasOppositeNormalwith(axis) || sf->hasSameNormalwith(axis)){
            delete sf;
            this->surfacesList.erase(this->surfacesList.begin() + i);
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

void Space::rotateSpaceByFloorTo00(){
    cout << " ---------- rotate -------------" << endl;
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    Surface* floor = findFirstSurfaceSimilarWithAxis(2);
    floor->setMBB();

    Plane_3 plane(Point_3(0,0,0), floor->av_normal);
    floor->makePlanar(plane);
    Vector_3 vector_z(0,0,1);
    double angle = -CGALCalculation::getAngle(floor->av_normal, vector_z);
    Vector_3 unit_vector = CGAL::cross_product(vector_z, floor->av_normal);
    unit_vector = unit_vector / sqrt(unit_vector.squared_length());
    cout << "rotate " << angle << ", " << unit_vector.squared_length()<< endl;
    assert(unit_vector.squared_length() < 1.000001 && unit_vector.squared_length() > 0.99999);

    double cos_value = cos(angle * PI /180.0);
    double sin_value = sin(angle * PI /180.0);

    double ux = unit_vector.x();
    double uy = unit_vector.y();
    double uz = unit_vector.z();

    Transformation rotateZ(cos_value + ux*ux *(1-cos_value), ux*uy*(1-cos_value) - (uz * sin_value), ux*uz*(1-cos_value) + uy*sin_value,
                            uy*ux*(1-cos_value) + uz * sin_value,cos_value + uy*uy*(1-cos_value), uy*uz*(1-cos_value)- (ux*sin_value),
                            uz*ux*(1-cos_value)-uy*sin_value , uz*uy*(1-cos_value) + ux * sin_value, cos_value + uz*uz*(1-cos_value),
                            1);

    for (ull i = 0 ; i < this->p_vertexList->size() ; i++){
        this->p_vertexList->at(i)->used = false;
    }

    for (ull i = 0 ; i < this->surfacesList.size() ; i++){
        for (ull j = 0 ; j < this->surfacesList[i]->v_list.size() ; j++){
            if (!this->surfacesList[i]->v_list[j]->used){
                this->surfacesList[i]->v_list[j]->used = true;
                Point_3 p = this->surfacesList[i]->v_list[j]->getCGALPoint();
                p = p.transform(rotateZ);
                this->surfacesList[i]->v_list[j]->setCoords(p.x(), p.y(), p.z());
            }
        }
    }
}
