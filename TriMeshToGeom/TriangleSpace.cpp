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

int TriangleSpace::makePolygonsGreedy(Checker* check){
    int combined_count = 0;
    vector<Triangle*> p_triangles;
    for (int i = 0 ; i < this->triangles.size(); i++){
        p_triangles.push_back(&this->triangles[i]);
    }
    pushCombinedPolygons(p_triangles, check, combined_count);
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
        pushCombinedPolygons(poly_set[dir], check, combined_count);
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
                pl_nv = pl_nv + normal;
                checked[index2] = true ;
            }
            //TODO
        }
        pushCombinedPolygons(candidates, check, combined_count);
        candidates.clear();
    }

    this->triangles.clear();

    cout << "\ndone make Polygons" << endl;
    return 0;
}


void TriangleSpace::printProcess(ull index, ull size){
    cout << "\r==========" << (int)((double)index/(double)size * 100) <<"% ========";
}


void TriangleSpace::pushCombinedPolygons(vector<Triangle*>& tri_list, Checker* check, int& combined_count)
{
    ull size = tri_list.size();
    bool* checked = (bool*)malloc(sizeof(bool) * size);
    std::fill(checked, checked + size, false);

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
            newcp = makeOneBigPolygon(tri_list, newcp, checked, id, check);
            if (newcp == NULL) break;
            if (combined_count % 250 == 0 )
            {
                cout << "\n------------ " << index << " -----------  :  " << id << "/" << size << endl;
                this->printProcess(combined_count, this->triangles.size());
            }
            combined_count++;
        }
        if (newcp != NULL) this->polygon_list.push_back(newcp);
    }
    free(checked);
}


CombinedPolygon* TriangleSpace::makeOneBigPolygon(vector<Triangle*> tri_list, CombinedPolygon* cp, bool* checked, int& id, Checker* check)
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

