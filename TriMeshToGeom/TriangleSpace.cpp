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
int TriangleSpace::makePolygons(Checker* check)
{
    // Separation by Normal
    vector<vector<Triangle*>> poly_set;
    poly_set = separateByNormal_6(this->triangles);
    if (poly_set.size() != 6 ) return -1;

    int combined_count = 0;
    for (unsigned int dir = 0 ; dir < 6 ; dir++)
    {
        vector<Triangle*> tri_list = poly_set[dir];
        unsigned long size = tri_list.size();
        bool* checked = (bool*)malloc(sizeof(bool) * size);
        std::fill(checked, checked + size, false);

        cout << "\n number : " << size << endl;

        for (unsigned long index = 0 ; index < size; index++)
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
                if (combined_count % 250 == 0 ){
                    cout << "\n------------" << index << "-----------" << endl;
                    this->printProcess(combined_count, this->triangles.size());
                }
                combined_count++;
            }
            if (newcp != NULL) this->polygon_list.push_back(newcp);
        }
        free(checked);
    }

    cout << "\ndone make Polygons" << endl;
    return 0;
}


void TriangleSpace::printProcess(unsigned long index, unsigned long size){
    cout << "\r==========" << (int)((double)index/(double)size * 100) <<"% ========";
}


CombinedPolygon* TriangleSpace::makeOneBigPolygon(vector<Triangle*> tri_list, CombinedPolygon* cp, bool* checked, int& id, Checker* check)
{
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (unsigned long i = 0 ; i < tri_list.size() ; i++, id++)
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

    unsigned long size = triangles.size();
    for (unsigned long index = 0 ; index < size; index++){
        Vector_3 normal = triangles[index].getNormal();
        int type = VectorCalculation::findNormalType(normal);
        ret[type].push_back(&triangles[index]);
    }

    return ret;
}

vector<vector<Triangle*>> TriangleSpace::separateByNormal_6()
{
    return separateByNormal_6(this->triangles);
}

void TriangleSpace::cleaning(Checker* check)
{

    for (int i = 0 ; i < (int)this->polygon_list.size() ; i++)
    {
        //make Each Polygon Coplanar
        this->polygon_list[i]->makeCoplanar();

        //this->polygon_list[i]->simplify_colinear();
    }

    //Classification

}




