#include "TriangleSpace.h"

#include <cstdio>

TriangleSpace::TriangleSpace(string pname, Checker* check)
{
    checker = check;
    name = pname;
}

TriangleSpace::~TriangleSpace()
{
    //dtor
}

void TriangleSpace::pushTriangle(Triangle tri){
    this->triangles.push_back(tri);
}

int TriangleSpace::makePolygonsGreedy(){
    int combined_count = 0;
    vector<Triangle*> p_triangles;
    ull size = this->triangles.size();
    for (ull i = 0 ; i < size; i++){
        p_triangles.push_back(&this->triangles[i]);
    }

    bool* checked = (bool*)malloc(sizeof(bool) * size);
    std::fill(checked, checked + size, false);

    vector<CombinedPolygon*> c_list = makePolygonsInList(p_triangles, checked, combined_count);
    this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());

    free(checked);

    this->triangles.clear();

    cout << "\ndone make Polygons" << endl;
    return 0;
}

int TriangleSpace::makePolygonsBySeparation()
{
    // Separation by Normal
    vector<vector<Triangle*>> poly_set;
    poly_set = separateByNormal_6(this->triangles);
    if (poly_set.size() != 6 ) return -1;

    int combined_count = 0;
    for (unsigned int dir = 0 ; dir < 6 ; dir++)
    {
        ull c_size = poly_set[dir].size();
        bool* checked = (bool*)malloc(sizeof(bool) * c_size);
        std::fill(checked, checked + c_size, false);

        vector<CombinedPolygon*> c_list = makePolygonsInList(poly_set[dir], checked, combined_count);
        this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());

        free(checked);
    }

    this->triangles.clear();

    cout << "\ndone make Polygons" << endl;
    return 0;
}

int TriangleSpace::makePolygonsByCandidator()
{
    ull size = this->triangles.size();
    bool* checked = (bool*)malloc(sizeof(bool) * size);
    std::fill(checked, checked + size, false);

    int combined_count = 0;
    for (ull index = 0 ; index < size; index++)
    {
        if (checked[index])
        {
            continue;
        }
        checked[index] = true ;

        Vector_3 pl_nv = this->triangles[index].getNormal();
        vector<Triangle*> candidates;
        for (ull index2 = 0 ; index2 < size ; index2++){
            if (checked[index2])
            {
                continue;
            }

            Vector_3 normal = this->triangles[index2].getNormal();
            if (checker->isSamePlanar(pl_nv, normal)){
                candidates.push_back(&this->triangles[index2]);
                //pl_nv = pl_nv + normal;
                checked[index2] = true ;
            }
        }

        ull c_size = candidates.size();
        bool* checked2 = (bool*)malloc(sizeof(bool) * c_size);
        std::fill(checked2, checked2 + c_size, false);

        vector<CombinedPolygon*> c_list = makePolygonsInList(candidates, checked2, combined_count);
        this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());

        free(checked2);

        candidates.clear();
    }

    this->triangles.clear();

    cout << "\ndone make Polygons" << endl;
    return 0;
}


void TriangleSpace::printProcess(ull index, ull size){
    cout << "\r==========" << (int)((double)index/(double)size * 100) <<"% ========";
}


vector<CombinedPolygon*> TriangleSpace::makePolygonsInList(vector<Triangle*>& tri_list, bool* checked, int& combined_count)
{
    vector<CombinedPolygon*> result_list;
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
        CombinedPolygon* newcp = new CombinedPolygon(tri_list[index]);

        ull count = 1 ;
        while (count != 0)
        {
            newcp = attachTriangle(tri_list, newcp, checked, count);
            if (newcp == NULL) break;
            if (combined_count % 250 == 0 )
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


CombinedPolygon* TriangleSpace::attachTriangle(vector<Triangle*> tri_list, CombinedPolygon* cp, bool* checked, ull& count)
{
    count = 0;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = 0 ; id < tri_list.size() ; id++)
    {
        if (!checked[id])
        {
            if (cp->attachTriangle(tri_list[id], checker))
            {
                checked[id] = true;
                count++;
            }
        }
    }
    return cp;
}

vector<vector<Triangle*>> TriangleSpace::separateByNormal_6(vector<Triangle>& triangles)
{
    vector<vector<Triangle*>> ret(6, vector<Triangle*>());

    ull size = triangles.size();
    for (ull index = 0 ; index < size; index++){
        Vector_3 normal = triangles[index].getNormal();
        int type = CGALCalculation::findNormalType(normal);
        ret[type].push_back(&triangles[index]);
    }

    return ret;
}

int TriangleSpace::makePolygonsCoplanar()
{
    for (int i = 0 ; i < (int)this->polygon_list.size() ; i++)
    {
        //make Each Polygon Coplanar
        this->polygon_list[i]->makeCoplanar();
    }
    return 0;
}


int TriangleSpace::combineCombinedPolygon(){
    cout << "Combine Polygons" << endl;
    //TODO
    ull p_size = this->polygon_list.size();
    bool* checked = (bool*)malloc(sizeof(bool) * p_size);
    std::fill(checked, checked + p_size, false);

    vector<CombinedPolygon*> new_poly_list;
    int combined_count = 0;
    for (ull i = 0 ; i < this->polygon_list.size() ; i++)
    {
        if (checked[i]) continue;
        checked[i] = true;

        ull count = -1;
        CombinedPolygon* newcp = new CombinedPolygon(this->polygon_list[i]);
        while(count != 0){
            newcp = attachPolygons(newcp, checked, count);
            if (newcp == NULL) break;
            if (combined_count % 50 == 0 )
            {
                this->printProcess(combined_count, this->polygon_list.size());
            }
            combined_count += count;
        }
        if (newcp != NULL) new_poly_list.push_back(newcp);
    }

    freeCombinedPolygons();
    this->polygon_list = new_poly_list;
    return 0;
}

CombinedPolygon* TriangleSpace::attachPolygons(CombinedPolygon* cp, bool* checked, ull& count)
{
    count = 0;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = 0 ; id < this->polygon_list.size() ; id++)
    {
        if (!checked[id])
        {
            if (cp->attachPolygon(this->polygon_list[id], checker))
            {
                checked[id] = true;
                count++;
            }
        }
    }
    return cp;
}

void TriangleSpace::freeCombinedPolygons(){
    for (ull id = 0 ; id < this->polygon_list.size() ; id++)
    {
        delete(this->polygon_list[id]);
    }
    this->polygon_list.clear();
}
