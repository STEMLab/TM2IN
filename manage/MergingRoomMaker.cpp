#define __DEBUG__
#include "manage/MergingRoomMaker.h"
#include <compute/SurfacesListComputation.h>
#include <fileio/export/MeshExporter.h>
#include "compute/SurfaceComputation.h"
#include "cgal/SurfaceHoleCover.h"
#include <ctime>

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

int MergingRoomMaker::pre_process() {
    if (this->menifestTriangleMesh()) return -1;
    if (this->convertTriangleMeshToSpace()) return -1;

    return 0;
}

int MergingRoomMaker::constructSpace() {
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

    makeSurfaceGraph();
    cout << "\n\ncheck Self Intersection? (y/n)" << endl;
    cin >> doCheckSelfIntersection;
    if (checkAnswer(doCheckSelfIntersection, 'y'))
        if (this->checkSelfIntersection()) return -1;

    cout << "\n\nre-triangulation" << endl;
    if (this->triangulation()) return -1;

    return 0;
}


int MergingRoomMaker::finish() {
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
            if (triangleMesh->checkClosedSurface())
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


void MergingRoomMaker::makeSurfaceGraph() {
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

int MergingRoomMaker::partitionTriangleMeshByComponent() {
    int i = 0;
    vector<TriangleMesh*> new_mesh_list;
    while ( i < this->mesh_list.size() ){
        assert(this->mesh_list[i]->checkClosedSurface());
        int result = this->mesh_list[i]->partitionByComponent(new_mesh_list);
        if(result == -1) return -1;
        else
            i++;
    }
    this->mesh_list = new_mesh_list;
    cout << "The Number of Mesh : " << this->mesh_list.size() << endl;

    return 0;
}

int MergingRoomMaker::menifestTriangleMesh() {
    clock_t begin = clock();
    for (int i = 0 ; i < this->mesh_list.size() ; i++){
        this->mesh_list[i]->init();
        if (!this->mesh_list[i]->checkClosedSurface()){
            cerr << i << " : it is not composed of closed surface"<< endl;
        }
    }
    clock_t end = clock();

    #ifdef __DEBUG__
        cout << "make graph time : " << double(end - begin) / CLOCKS_PER_SEC << "s" << endl;
    #else

    #endif

    if (this->partitionTriangleMeshByComponent()) return -1;

    if (this->remainStructure()) return -1;

    return false;
}

int MergingRoomMaker::remainStructure() {
    cerr << "TODO : remainStructure" << endl;
    // this->mesh_list.erase(this->mesh_list.begin() + 1, this->mesh_list.end());

    return 0;
}


int MergingRoomMaker::mergeSurfaces() {
    cout << "Enter Start Degree of merging(Default : 1.0)" << endl;
#ifdef __DEBUG__
    this->startDegree = 0.001;
#else
    cin >> this->startDegree;
#endif
    for (ull it = 0 ; it < this->spaceList.size(); it++)
    {
        Space* space = this->spaceList[it];
        this->generation_writer->start(space);

        // check duplicate coordinates
        if (this->spaceList[it]->checkDuplicateVertexInSurfaces()) return -1;

        // limit degree of same normal vector angle
        double degree = startDegree;
        if (processGenerations(space, degree)) return -1;

        if (space->checkSurfaceValid() == -1){ cout << "Surface is not valid" << endl; return -1; }
        space->sortSurfacesByArea();
        space->tagID();
    }
    return 0;
}

int MergingRoomMaker::processGenerations(Space *space, double &degree) {
    ll p_size = space->surfacesList.size();
    while (true){
        assert(p_size > 0);
        cout << "generation " << space->generation << ": " << space->surfacesList.size()<< endl;
        if (space->combineSurface(degree) == -1){
            cerr << "combine error" << endl;
            return -1;
        }

        if (space->checkSurfaceValid() == -1){
            cerr << "Surface is not valid" << endl;
            return -1;
        }

        if (p_size == (int)space->surfacesList.size()) {
            cout << "generation " << space->generation  << " done..\n\n\n"<< endl;
            break;
        }
        else p_size = (int)space->surfacesList.size();

        this->generation_writer->write();
        if (degree < 15) degree += 0.05;

        space->generation++;
    }
    return 0;
}


int MergingRoomMaker::triangulation() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        cout << "space : " << it << endl;
        Space *space = this->spaceList[it];
        space->triangulateSurfaces();
    }
    return 0;
}

int MergingRoomMaker::checkSelfIntersection() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        Space *space = this->spaceList[it];
        space->checkSelfIntersection();
    }
    return 0;
}

int MergingRoomMaker::simplifyShareEdge() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        Space *space = this->spaceList[it];
        cout << "simplify space " << space->name << endl;
        space->simplifySegment();
        // space->removeStraight();
    }
    return 0;
}


