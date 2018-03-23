#include "manage/MergingRoomMaker.h"
#include <compute/SurfacesListComputation.h>
#include "compute/SurfaceComputation.h"
#include "cgal/SurfaceHoleCover.h"

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
    if (this->resolveWrongTriangle()) return -1;

    if (this->constructMeshGraph()) return -1;

    if (this->convertTriangleMeshToSpace()) return -1;

    return 0;
}

int MergingRoomMaker::constructSpace() {
    assert (this->spaceList.size() != 0);

    char doNotMerge, doCheckSelfIntersection;
    cout << "keep Triangle without merging? (y/n)" << endl;
    cin >> doNotMerge;
    if (checkAnswer(doNotMerge, 'y')){
        return 0;
    }
    if (this->mergeSurfaces()) return -1;

    if (this->simplifyShareEdge()) return -1;

    cout << "check Self Intersection? (y/n)" << endl;
    cin >> doCheckSelfIntersection;
    if (checkAnswer(doCheckSelfIntersection, 'y'))
        if (this->checkSelfIntersection()) return -1;

    cout << "re-triangulation" << endl;
    if (this->triangulation()) return -1;

    return 0;
}


int MergingRoomMaker::finish() {
    char doExport3DS;
    this->exportSpace();

    cout <<"Export 3DS?" << endl; cin >>doExport3DS;
    if (checkAnswer(doExport3DS, 'y')){
        if (this->convertSpaceToTriangleMesh()) return -1;
        if (this->constructMeshGraph()) return -1;
        this->mesh->export3DS((paths["versionDir"] + paths["filename"] + ".3DS").c_str());
    }

    return 0;
}

int MergingRoomMaker::remainStructure() {
    cerr << "TODO" << endl;
    return 0;
}

bool MergingRoomMaker::resolveWrongTriangle() {
    int count = 0;
    while (this->mesh->resolveWrongTriangle()){
        cout << count++ << ", ";
    }
    return false;
}

int MergingRoomMaker::mergeSurfaces() {
    cout << "Enter Start Degree of merging(Default : 1.0)" << endl;
    cin >> this->startDegree;

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

        /*
        if (snap_mode){
            double diff = 0.0001;
            if (space->snapSurface(diff) == -1){ cout << "snap Surface" << endl; return -1;}
            if (processGenerations(space, gen, degree)) return -1;
            if (space->checkSurfaceValid() == -1){ cout << "cannot handle defect" << endl; return -1; }
        }
        */
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

int MergingRoomMaker::updateVertexList(){
    vector<Vertex*> newVertices;
    for (ull it = 0 ; it < this->spaceList.size() ; it++){
        Space* space = this->spaceList[it];
        space->putVerticesAndUpdateIndex(newVertices);
    }
    this->vertices = newVertices;
    return 0;
}

int MergingRoomMaker::rotateSurfaces(){
    for (ull it = 0 ; it < this->spaceList.size(); it++)
    {
        Space* space = this->spaceList[it];
        space->rotateSpaceByFloorTo00();
        if (space->translateSpaceToOrigin() == -1){
            cout << "translateSpaceToOrigin error" << endl;
            return -1;
        }
    }
    return 0;
}

int MergingRoomMaker::triangulation() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        cout << "space : " << it << endl;
        Space *space = this->spaceList[it];
        // Triangulation
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

int MergingRoomMaker::constructMeshGraph() {
    char doConstructMeshGraph;
    cout << "Do you want to make graph about mesh?" << endl;
    cin >> doConstructMeshGraph;

    if (checkAnswer(doConstructMeshGraph, 'y')){
        this->mesh->makeGraph();
        if (!this->mesh->checkClosedSurface()){
            cerr << "it is not composed of closed surface"<< endl;
            return -1;
        }
        char doGroupByComponent;
        cout << "Group By Component?" << endl;
        cin >> doGroupByComponent;
        if (checkAnswer(doGroupByComponent, 'y'))
            if (this->mesh->groupByClosedSurface()) return -1;

        char doRemainStructure;
        cout << "Remain only Indoor Structure?" << endl;
        cin >> doRemainStructure;
        if (checkAnswer(doRemainStructure, 'y'))
            if (this->remainStructure()) return -1;
    }

    return 0;
}

int MergingRoomMaker::simplifyShareEdge() {
    //construct Surface Graph

    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        Space *space = this->spaceList[it];
        cout << "simplify space " << space->name << endl;
        space->simplifySegment();
        space->removeStraight();
    }
    return 0;
}


