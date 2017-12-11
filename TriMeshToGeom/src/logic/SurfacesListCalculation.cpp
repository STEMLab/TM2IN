#include "SurfacesListCalculation.h"

#include "data/SurfaceGraph.h"

using namespace std;

Surface* SurfacesListCalculation::findFirstSurfaceSimilarWithAxis(vector<Surface*>& surfacesList, int axis){
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        Surface* sf = surfacesList[i];
        if (CGALCalculation::findNormalType6(sf->av_normal) == axis){
            return sf;
        }
    }
    assert(false);
}

int SurfacesListCalculation::findFirstSurfaceIndexSimilarWithAxis(vector<Surface*>& surfacesList, int axis){
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        Surface* sf = surfacesList[i];
        if (CGALCalculation::findNormalType6(sf->av_normal) == axis){
            return i;
        }
    }
    assert(false);
}

SurfaceGraph* SurfacesListCalculation::makeGraph(vector<Surface*>& surfacesList){
    sort(surfacesList.begin(), surfacesList.end(), Surface::compareArea);
    SurfaceGraph* surface_graph = new SurfaceGraph();
    surface_graph->makeAdjacentGraph(surfacesList);
    return surface_graph;
}

void SurfacesListCalculation::tagID(vector<Surface*>& surfacesList){
    for (ull i = 0 ; i < (ull)surfacesList.size() ; i++)
    {
        surfacesList[i]->sf_id = i;
    }
}

void SurfacesListCalculation::getMBB(vector<Surface*>& surfacesList, vector<vector<double>>& v_minmax){
    v_minmax.clear();
    v_minmax.assign(2, vector<double>());

    for (int i = 0 ; i < 3 ; i++)
    {
        v_minmax[1].push_back(-100000.0000);
        v_minmax[0].push_back(100000.0000);
    }

    for (ull i = 0 ; i < surfacesList.size() ; i++){
        Surface* cp = surfacesList[i];
        cp->setMBB();
        for (int j = 0 ; j < 3 ; j++){
            v_minmax[1][j] = max(v_minmax[1][j], cp->max_coords[j]);
            v_minmax[0][j] = min(v_minmax[0][j], cp->min_coords[j]);
        }
    }

    return ;
}
