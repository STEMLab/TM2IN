//
// Created by dongmin on 18. 7. 19.
//

#include "converter/Converter.h"


int Converter::mergeSurfaces() {
    for (ull it = 0 ; it < this->spaceList.size(); it++)
    {
        PolyhedralSurface* space = this->spaceList[it];
        if (this->generation_writer) this->generation_writer->start(space);
        space->generation++;

        // check duplicate coordinates
        if (this->spaceList[it]->checkDuplicateVertexInSurfaces()) return -1;

        // limit degree of same normal vector angle

        if (processGenerations(space)) return -1;

        Checker::threshold_2 = 10.0;
        Checker::threshold_1 = 40.0;
        if (processGenerations(space)) return -1;

        if (space->checkSurfaceValid() == -1){ cout << "Surface is not valid" << endl; return -1; }
        space->sortSurfacesByArea();
    }
    return 0;
}


void Converter::makeSurfaceGraph() {
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


int Converter::doValidation() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        PolyhedralSurface *space = this->spaceList[it];
        space->checkSelfIntersection();
    }
    return 0;
}


int Converter::simplifyShareEdge() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        PolyhedralSurface *space = this->spaceList[it];
        cout << "simplify space " << space->name << endl;
        space->simplifySegment();
        // space->removeStraight();
    }
    return 0;
}


int Converter::triangulation() {
    cout << "\n\nTriangulationConverter::re-triangulation" << endl;
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        cout << "space : " << it << endl;
        PolyhedralSurface *space = this->spaceList[it];
        space->triangulateSurfaces();
    }
    return 0;
}


int Converter::processGenerations(PolyhedralSurface *space) {
    ll p_size = (ull)space->surfacesList.size();
    while (true){
        assert(p_size > 0);
        cout << "generation " << space->generation << ": " << space->surfacesList.size()<< endl;
        cout << "degree  : " << Checker::threshold_1 << endl;
        int mergeSurface = space->mergeSurface();
        if (mergeSurface == -1){
            cerr << "combine error" << endl;
            return -1;
        }
        int simplifySegment = 0;
        if (!mergeSurface)
            simplifySegment = space->simplifySegment();
        if (simplifySegment == -1){
            return -1;
        }
        if (space->checkSurfaceValid() == -1){
            cerr << "Surface is not valid" << endl;
            return -1;
        }

        if (mergeSurface || simplifySegment){
            p_size = (int)space->surfacesList.size();
        }
        else{
            cout << "generation " << space->generation << " done..\n\n\n"<< endl;
            break;
        }

        if (this->generation_writer) this->generation_writer->write();
        if (Checker::threshold_1 < 40) Checker::threshold_1 += 2.0;

        space->generation++;
        space->updateNormal();
    }
    return 0;
}

int Converter::polygonize() {
    TM2IN::algorithm::Polygonizer* polygonizer = create_polygonizer();
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        PolyhedralSurface *space = this->spaceList[it];
        polygonizer->run(space);
    }
    return 0;
}


TM2IN::algorithm::Polygonizer *Converter::create_polygonizer() {
    switch(options.polygonizer_mode){
        case 1:
            return new TM2IN::algorithm::PCAPolygonizer();
        case 2:
            return new TM2IN::algorithm::TrianglePolygonizer();
        case 3:
            return new TM2IN::algorithm::DividedPolygonizer();
    }
    throw std::runtime_error("options.polygonizer_mode has wrong value");
}
