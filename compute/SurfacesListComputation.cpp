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
            v_minmax[1][j] = max(v_minmax[1][j], cp->max_coords[j]);
            v_minmax[0][j] = min(v_minmax[0][j], cp->min_coords[j]);
        }
    }

    CGAL::Bbox_3 bbox_3(v_minmax[0][0],v_minmax[0][1], v_minmax[0][2], v_minmax[1][0], v_minmax[1][1], v_minmax[1][2]);
    return bbox_3;
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

bool TMIC::mergeSurfaces(vector<Surface *> &surfacesList, vector<Surface*>& result) {
    result.clear();
    //deep copy
    for (ull i = 0 ; i < surfacesList.size() ; i++){
        result.push_back(new Surface(surfacesList[i]));
    }

    bool hasMerged = false;
    bool isMerged = true;
    ull combined_count = 0;
    while (isMerged){
        sort(result.begin(), result.end(), Surface::compareLength);
        if (result.size() == 1) break;
        for (ull i = 0 ; i < result.size() - 1 ; i++){
            ull j = i + 1;
            isMerged = false;
            while (j < result.size()){
                if (TMIC::combine(result[i], result[j]) == 0)
                {
                    cout << ".";
                    combined_count++;
                    isMerged = true;
                    hasMerged = true;
                    result.erase(result.begin() + j);
                } else
                    j++;
            }
            if (isMerged) i -= 1;
            printProcess(combined_count, surfacesList.size(), "mergeSurface");
        }
    }

    return hasMerged;
}

bool TMIC::mergeSurfaces(vector<Triangle *> &triangleList, vector<Surface*>& result) {
    vector<Surface *> surfacesList;
    for (Triangle* tri : triangleList)
        surfacesList.push_back(new Surface(tri));
    bool hasMerged = TMIC::mergeSurfaces(surfacesList, result);
    for(Surface* sf : surfacesList)
        delete sf;
    return hasMerged;
}

double TMIC::getAverageSize(const vector<Surface *> &surfacesList) {
    double wholeArea = 0.0;

    for (Surface* sf : surfacesList){
        wholeArea += sf->getArea();
    }

    return wholeArea / (double)surfacesList.size();
}
