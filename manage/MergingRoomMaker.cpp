#include "manage/MergingRoomMaker.h"
#include <compute/SurfacesListComputation.h>
#include "cgal/SurfaceHoleCover.h"

using namespace std;


int MergingRoomMaker::pre_process() {
    return 0;
}

int MergingRoomMaker::constructSpace() {
    assert (this->space_list.size() != 0);

    if (this->mergeSurfaces()) return -1;

    // make Surfaces Planar
    if (this->makeSurfacesPlanar()) return -1;

    // remove Self-intersection in one Surface.

    // fill Hole
    if (this->fillHoleWithUsingPolyhedralSurface()) return -1;

    return 0;
}


int MergingRoomMaker::finish(){
    return 0;
}




int MergingRoomMaker::mergeSurfaces() {
    double startDegree = this->startDegree;
    bool simplify_mode = this->simplifyLine;
    bool snap_mode = this->snapMode;
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        Space* space = this->space_list[it];
        this->generation_writer->start(space);

        // check duplicate coords
        if (this->space_list[it]->checkDuplicateVertexInSurfaces()) return -1;

        // limit degree of same normal vector angle
        double degree = startDegree;
        int gen = 0;
        double angleInDefect = 0.1;
        ll sizeBeforeCombine = space->surfacesList.size();
        while (true){
            if (processGenerations(space, gen, degree)) return -1;

            // loop is done when nothing can be merged.
            if (sizeBeforeCombine == (int)space->surfacesList.size()) {
                break;
            } else
                sizeBeforeCombine = (int)space->surfacesList.size();

            if (simplify_mode)
                if (space->simplifySegment() == -1){ cout << "simplify error" << endl; return -1;}
            if (angleInDefect < 1.0) angleInDefect += 0.1;
            if (space->handleDefect(angleInDefect) == -1){ cout << "cannot handle defect" << endl; return -1; }
        }

        sort(space->surfacesList.begin(), space->surfacesList.end(), Surface::compareArea);
        SLC::tagID(space->surfacesList);

        if (snap_mode){
            double diff = 0.0001;
            if (space->snapSurface(diff) == -1){ cout << "snap Surface" << endl; return -1;}
            if (processGenerations(space, gen, degree)) return -1;
            if (space->handleDefect(angleInDefect) == -1){ cout << "cannot handle defect" << endl; return -1; }
        }
    }
}

int MergingRoomMaker::processGenerations(Space *space, int &currentGeneration, double &degree){
    ll p_size = space->surfacesList.size();
    while (true){
        cout << "generation " << currentGeneration << ": " << space->surfacesList.size()<< endl;
        if (space->combineSurface(degree) == -1)
        {
            cout << "combine error" << endl;
            return -1;
        }

        if (p_size == (int)space->surfacesList.size()) {
            cout << "generation " << currentGeneration  << " done.. "<< endl;
            break;
        }
        else p_size = (int)space->surfacesList.size();

        currentGeneration++;
        this->generation_writer->write();
        if (degree < 15) degree += 0.05;
    }
    return 0;
}


int MergingRoomMaker::fillHoleWithUsingPolyhedralSurface() {
    vector<Vertex*> newVertices;
    for (ull it = 0 ; it < this->space_list.size() ; it++){
        vector<Vertex*> spaceVertices = SurfaceHoleCover::fillHole(this->vertices, this->space_list[it]->surfacesList);
        newVertices.insert(newVertices.end(), spaceVertices.begin(), spaceVertices.end());
    }
    this->vertices = newVertices;
    return 0;
}


int MergingRoomMaker::rotateSurfaces(){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        Space* space = this->space_list[it];
        space->rotateSpaceByFloorTo00();
        if (space->match00() == -1){
            cout << "match00 error" << endl;
            return -1;
        }
    }
    return 0;
}

int MergingRoomMaker::makeSurfacesPlanar() {
    vector<Vertex*> newVertices;
    for (ull it = 0 ; it < this->space_list.size() ; it++){
        Space* space = this->space_list[it];
    }
    this->vertices = newVertices;
    return 0;
}
