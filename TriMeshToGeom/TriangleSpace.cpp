#include "TriangleSpace.h"

#include <cstdio>

TriangleSpace::TriangleSpace()
{
    //ctor
}

TriangleSpace::~TriangleSpace()
{
    //dtor
}

void TriangleSpace::pushTriangle(Triangle tri){
    this->triangles.push_back(tri);
}

int TriangleSpace::makePolygonsGreedy(Checker* check){
    int combined_count = 0;
    vector<Triangle*> p_triangles;
    ull size = this->triangles.size();
    for (int i = 0 ; i < size; i++){
        p_triangles.push_back(&this->triangles[i]);
    }

    bool* checked = (bool*)malloc(sizeof(bool) * size);
    std::fill(checked, checked + size, false);

    vector<CombinedPolygon*> c_list = makePolygonsInList(p_triangles, checked, check, combined_count);
    this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());
    free(checked);
    this->triangles.clear();

    cout << "\ndone make Polygons" << endl;
    return 0;
}

int TriangleSpace::makePolygonsBySeparation(Checker* check)
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

        vector<CombinedPolygon*> c_list = makePolygonsInList(poly_set[dir], checked, check, combined_count);
this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());
        free(checked);
    }

    this->triangles.clear();

    cout << "\ndone make Polygons" << endl;
    return 0;
}

int TriangleSpace::makePolygonsByCandidator(Checker* check)
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
            if (check->isSimilarOrientation(pl_nv, normal)){
                candidates.push_back(&this->triangles[index2]);
                //pl_nv = pl_nv + normal;
                checked[index2] = true ;
            }
            //TODO
        }

        ull c_size = candidates.size();
        bool* checked2 = (bool*)malloc(sizeof(bool) * c_size);
        std::fill(checked2, checked2 + c_size, false);

        vector<CombinedPolygon*> c_list = makePolygonsInList(candidates, checked2, check, combined_count);
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


vector<CombinedPolygon*> TriangleSpace::makePolygonsInList(vector<Triangle*>& tri_list, bool* checked, Checker* check, int& combined_count)
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

        int id = 0;

        while (id < size)
        {
            id = 0;
            newcp = attachTriangle(tri_list, newcp, checked, id, check);
            if (newcp == NULL) break;
            if (combined_count % 250 == 0 )
            {
                cout << "\n------------ " << index << " -----------  :  " << id << "/" << size << endl;
                this->printProcess(combined_count, this->triangles.size());
            }
            combined_count++;
        }
        if (newcp != NULL) result_list.push_back(newcp);
    }
    return result_list;
}


CombinedPolygon* TriangleSpace::attachTriangle(vector<Triangle*> tri_list, CombinedPolygon* cp, bool* checked, int& id, Checker* check)
{
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull i = 0 ; i < tri_list.size() ; i++, id++)
    {
        if (!checked[i])
        {
            if (cp->attachTriangle(tri_list[i], check))
            {
                //printf("\r%lu               ",i);
                checked[i] = true;
                return cp;
            }
        }
    }
    return cp;
}

vector<vector<Triangle*>> TriangleSpace::separateByNormal_6(vector<Triangle>& triangles)
{
//    Vector_3 up(0,0,1);
//    Vector_3 down(0,0,-1);
//    Vector_3 right(1,0,0);
//    Vector_3 left(-1,0,0);
//    Vector_3 front(0,1,0);
//    Vector_3 back(0,-1,0);
    vector<vector<Triangle*>> ret(6, vector<Triangle*>());

    ull size = triangles.size();
    for (ull index = 0 ; index < size; index++){
        Vector_3 normal = triangles[index].getNormal();
        int type = VectorCalculation::findNormalType(normal);
        ret[type].push_back(&triangles[index]);
    }

    return ret;
}

int TriangleSpace::makePolygonsCoplanar(Checker* check)
{

    for (int i = 0 ; i < (int)this->polygon_list.size() ; i++)
    {
        //make Each Polygon Coplanar
        this->polygon_list[i]->makeCoplanar();
    }
    return 0;
}


int TriangleSpace::combineCombinedPolygon(Checker* checker){
    //TODO
    return 0;
}

