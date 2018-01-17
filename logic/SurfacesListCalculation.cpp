#include "SurfacesListCalculation.h"

#include "features/SurfaceGraph.h"

using namespace std;



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
