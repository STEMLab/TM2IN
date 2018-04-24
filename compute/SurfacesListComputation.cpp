#include <features/TriangleMesh.h>
#include "space_maker/OnlyWallSpaceMaker.h"
#include "SurfacesListComputation.h"

#include "features/SurfaceGraph.h"
#include "SurfaceComputation.h"
#include "Surface_pair_computation.h"

using namespace std;



void SurfacesListComputation::tagID(vector<Surface*>& surfacesList){
    for (ull i = 0 ; i < (ull)surfacesList.size() ; i++)
    {
        surfacesList[i]->sf_id = to_string(i);
    }
}

vector<vector<double>> SurfacesListComputation::getMBB(vector<Surface*>& surfacesList){
    vector<vector<double>> v_minmax;
    v_minmax.assign(2, vector<double>());

    for (int i = 0 ; i < 3 ; i++)
    {
        v_minmax[1].push_back(-100000.0000);
        v_minmax[0].push_back(100000.0000);
    }

    for (ull i = 0 ; i < surfacesList.size() ; i++){
        Surface* cp = surfacesList[i];
        cp->updateMBB();
        for (int j = 0 ; j < 3 ; j++){
            v_minmax[1][j] = max(v_minmax[1][j], cp->max_coords[j]);
            v_minmax[0][j] = min(v_minmax[0][j], cp->min_coords[j]);
        }
    }

    return v_minmax;
}

int SurfacesListComputation::findFirstSurfaceIndexSimilarWithAxis(vector<Surface*>& surfacesList, int axis){
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        Surface* sf = surfacesList[i];
        if (CGALCalculation::findNormalType6(sf->normal) == axis){
            return i;
        }
    }
    assert(false);
}

ull SurfacesListComputation::countTriangles(vector<TriangleMesh *> tm) {
    ull result = 0;
    for (TriangleMesh* mesh : tm){
        result += mesh->triangles.size();
    }
    return result;
}

vector<Surface *> TMIC::mergeSurfaces(vector<Surface *> &surfacesList) {
    vector<Surface*> newSurfacesList;
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        newSurfacesList.push_back(new Surface(surfacesList[i]));
    }

    bool isMerged = true;
    ull combined_count = 0;
    while (isMerged){
        sort(newSurfacesList.begin(), newSurfacesList.end(), Surface::compareLength);
        isMerged = false;
        for (ull i = 0 ; i < newSurfacesList.size() - 1 ; i++){
            for (ull j = i + 1 ; j < newSurfacesList.size() ;){
                if (TMIC::combine(newSurfacesList[i], newSurfacesList[j]) == 0)
                {
                    cout << ".";
                    combined_count++;
                    isMerged = true;
                    newSurfacesList.erase(newSurfacesList.begin() + j);
                } else
                    j++;
            }
            printProcess(combined_count, newSurfacesList.size(), "combineSurface");
        }
    }

    return newSurfacesList;
    /*
    ull p_size = surfacesList.size();
    bool* checked = (bool*)malloc(sizeof(bool) * p_size);
    std::fill(checked, checked + p_size, false);

    int combined_count = 0;
    for (ull i = 0 ; i < surfacesList.size() ; i++)
    {
        if (checked[i]) continue;
        checked[i] = true;

        ll count = -1;
        Surface* newcp = new Surface(this->surfacesList[i]);
        while(count != 0){
            newcp = attachSurfaces(newcp, i + 1, checked, count);
            if (newcp == NULL) break;
            printProcess(combined_count, this->surfacesList.size(), "combineSurface");
            combined_count += count;
        }
        if (newcp != NULL) newSurfacesList.push_back(newcp);
    }
    */
}

vector<Surface *> TMIC::mergeSurfaces(vector<Triangle *> &triangleList) {
    vector<Surface *> surfacesList;
    for (Triangle* tri : triangleList)
        surfacesList.push_back(new Surface(tri));
    return TMIC::mergeSurfaces(surfacesList);
}
