#include <features/TriangleMesh.h>
#include <cgal/vector_angle.h>
#include "SurfacesListComputation.h"

#include "SurfaceComputation.h"

using namespace std;



void SurfacesListComputation::tagID(vector<Surface*>& surfacesList){
    for (ull i = 0 ; i < (ull)surfacesList.size() ; i++)
    {
        surfacesList[i]->sf_id = to_string(i);
    }
}

CGAL::Bbox_3 TMIC::getMBB(vector<Surface *> &surfacesList){
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
            v_minmax[1][j] = max(v_minmax[1][j], cp->getMax_coords()[j]);
            v_minmax[0][j] = min(v_minmax[0][j], cp->getMin_coords()[j]);
        }
    }

    CGAL::Bbox_3 bbox_3(v_minmax[0][0],v_minmax[0][1], v_minmax[0][2], v_minmax[1][0], v_minmax[1][1], v_minmax[1][2]);
    return bbox_3;
}

int SurfacesListComputation::findFirstSurfaceIndexSimilarWithAxis(vector<Surface*>& surfacesList, int axis){
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        Surface* sf = surfacesList[i];
        if (TM2IN::cgal::findNormalType6(sf->normal) == axis){
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

double TMIC::getAverageSize(const vector<Surface *> &surfacesList) {
    double wholeArea = 0.0;

    for (Surface* sf : surfacesList){
        wholeArea += sf->getArea();
    }

    return wholeArea / (double)surfacesList.size();
}
