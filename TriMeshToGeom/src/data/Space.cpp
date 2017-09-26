#include "data/Space.h"

#include <cstdio>

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
            if (CleanPolygonMaker::combine(cp, this->surfacesList[id], checker, degree))
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
    double cp_area = cp->sq_area;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = start ; id < this->surfacesList.size() ; id++)
    {
        double id_area = this->surfacesList[id]->sq_area;
        if (!checked[id] && (id_area < 0.00001 || id_area * 1000 < cp_area))
        {
            if (CleanPolygonMaker::combine(cp, this->surfacesList[id], checker, degree))
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

    ull p_size = this->surfacesList.size();

    for (ull i = 0 ; i < p_size - 1; i++)
    {
        for (ull j = i + 1; j < p_size ; j++)
        {
            int loop_count = 0;
            while (CleanPolygonMaker::simplifyLineSegment(this->surfacesList[i], this->surfacesList[j]))
            {
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
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
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
            this->surfacesList.erase(this->surfacesList.begin() + i);
            cout << "Erase unvalid surface" << endl;
        }
    }
    return 0;
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
            i++;
        }
        else{
            this->p_vertexList->erase(this->p_vertexList->begin() + i);
            removed_count++;
        }
    }
    cout << "removed vertices : " << removed_count << endl;
    cout << "remained vertices : " << this->p_vertexList->size() << endl;;
    return 0;
}

int Space::makeSurfacesPlanar(Checker* ch){
    cout << "\n------------- make planar --------------\n" << endl;

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
            cout << "Erase unvalid surface in match00" << endl;
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

int Space::makeGraph(Checker* ch){
    cout << "\n------------- Graph --------------\n" << endl;
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    this->tagID();

    this->surface_graph = new SurfaceGraph();
    surface_graph->makeAdjacentGraph(this->surfacesList);

    surface_graph->print_bfs();
    return 0;
}



void Space::freeSurfaces(){
    for (ull id = 0 ; id < this->surfacesList.size() ; id++)
    {
        delete(this->surfacesList[id]);
    }
    this->surfacesList.clear();
}


//
//int TriangleSpace::makeSurfacesBySeparation()
//{
//    // Separation by Normal
//    vector<vector<Triangle*>> poly_set;
//    poly_set = separateByNormal_6(this->triangles);
//    if (poly_set.size() != 6 ) return -1;
//
//    int combined_count = 0;
//    for (unsigned int dir = 0 ; dir < 6 ; dir++)
//    {
//        ull c_size = poly_set[dir].size();
//        bool* checked = (bool*)malloc(sizeof(bool) * c_size);
//        std::fill(checked, checked + c_size, false);
//
//        vector<Surface*> c_list = makeSurfacesInList(poly_set[dir], checked, combined_count);
//        this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());
//
//        free(checked);
//    }
//
//    this->triangles.clear();
//
//    cout << "\ndone make Surfaces" << endl;
//    return 0;
//}
//
//int TriangleSpace::makeSurfacesByCandidator()
//{
//    ull size = this->triangles.size();
//    bool* checked = (bool*)malloc(sizeof(bool) * size);
//    std::fill(checked, checked + size, false);
//
//    int combined_count = 0;
//    for (ull index = 0 ; index < size; index++)
//    {
//        if (checked[index])
//        {
//            continue;
//        }
//        checked[index] = true ;
//
//        Vector_3 pl_nv = this->triangles[index].getNormal();
//        vector<Triangle*> candidates;
//        for (ull index2 = 0 ; index2 < size ; index2++){
//            if (checked[index2])
//            {
//                continue;
//            }
//
//            Vector_3 normal = this->triangles[index2].getNormal();
//            if (checker->isSamePlanar(pl_nv, normal)){
//                candidates.push_back(&this->triangles[index2]);
//                //pl_nv = pl_nv + normal;
//                checked[index2] = true ;
//            }
//        }
//
//        ull c_size = candidates.size();
//        bool* checked2 = (bool*)malloc(sizeof(bool) * c_size);
//        std::fill(checked2, checked2 + c_size, false);
//
//        vector<Surface*> c_list = makeSurfacesInList(candidates, checked2, combined_count);
//        this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());
//
//        free(checked2);
//
//        candidates.clear();
//    }
//
//    this->triangles.clear();
//
//    cout << "\ndone make Surfaces" << endl;
//    return 0;
//}
//
//
//vector<vector<Triangle*>> TriangleSpace::separateByNormal_6(vector<Triangle>& triangles)
//{
//    vector<vector<Triangle*>> ret(6, vector<Triangle*>());
//
//    ull size = triangles.size();
//    for (ull index = 0 ; index < size; index++){
//        Vector_3 normal = triangles[index].getNormal();
//        int type = CGALCalculation::findNormalType(normal);
//        ret[type].push_back(&triangles[index]);
//    }
//
//    return ret;
//}
