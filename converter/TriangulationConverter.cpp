#define __DEBUG__
#include "TriangulationConverter.h"
#include <compute/SurfacesListComputation.h>
#include <fileio/export/MeshExporter.h>
#include "compute/SurfaceComputation.h"
#include "cgal/SurfaceHoleCover.h"
#include "Converter.h"

using namespace std;

/**
 * @param a
 * @param b lower case. the answer what I expect.
 * @return
 */
bool checkAnswer(char a, char b){
    if (a == b || a == b + 32){
        return true;
    }
    return false;
}

int TriangulationConverter::pre_process() {
    if (this->menifestTriangleMesh()) return -1;
    if (this->convertTriangleMeshToSpace()) return -1;

    return 0;
}

int TriangulationConverter::constructSpace() {
    assert (this->spaceList.size() != 0);

    char doNotMerge, doCheckSelfIntersection;
    cout << "keep Triangle without merging? (y/n)" << endl;

    // cin >> doNotMerge;
    doNotMerge = 'n';
    cout << doNotMerge << endl;

    if (checkAnswer(doNotMerge, 'y')){
        return 0;
    }
    if (this->mergeSurfaces()) return -1;

    if (this->simplifyShareEdge()) return -1;

    this->makeSurfaceGraph();

    cout << "\n\ncheck Self Intersection? (y/n)" << endl;
    cin >> doCheckSelfIntersection;
    if (checkAnswer(doCheckSelfIntersection, 'y'))
        if (this->checkSelfIntersection()) return -1;

    cout << "\n\nre-triangulation" << endl;
    if (this->triangulation()) return -1;

    return 0;
}


int TriangulationConverter::finish() {
    this->tagID();
    this->exportSpace();

    char doConvertToMesh;
    cout << "Convert To Triangle Mesh?" << endl;
    cin >> doConvertToMesh;
    if (checkAnswer(doConvertToMesh, 'y')){
        if (this->convertSpaceToTriangleMesh()) return -1;
        this->spaceList.clear();
        for (int i = 0 ; i < this->mesh_list.size() ; i++){
            TriangleMesh *&triangleMesh = this->mesh_list[i];
            triangleMesh->init();
            cout << "\n\n" << i << "th mesh" << endl;
            if (triangleMesh->checkClosed())
                cout << "this mesh is closed\n\n" << endl;
        }

        char doExport3DS;
        cout <<"Export 3DS?" << endl; cin >>doExport3DS;
        if (checkAnswer(doExport3DS, 'y')){
            this->export3DS((paths["versionDir"] + paths["filename"] + ".3DS").c_str());
        }
    }

    return 0;
}


void TriangulationConverter::makeSurfaceGraph() {
    for (int i = 0 ; i < spaceList.size() ; i++){
        cout << "\n\n" << i << "th graph" << endl;
        spaceList[i]->surfaceGraph = new SurfaceGraph();
        spaceList[i]->surfaceGraph->makeAdjacentGraph(spaceList[i]->surfacesList);
        if (spaceList[i]->surfaceGraph->isClosedSurface()){
            cout << "this is closed" << endl;
        }
        else{
            cout << "not closed" << endl;
        }
        cout << "------------\n" << endl;
    }
}


int TriangulationConverter::triangulation() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        cout << "space : " << it << endl;
        Space *space = this->spaceList[it];
        space->triangulateSurfaces();
    }
    return 0;
}


