//
// Created by dongmin on 18. 7. 19.
//

#include <algorithm/merge_surfaces.h>
#include "converter/Converter.h"


int Converter::mergeSurfaces() {
    for (ull it = 0 ; it < this->spaceList.size(); it++)
    {
        PolyhedralSurface* space = this->spaceList[it];
        if (this->generation_writer) this->generation_writer->start(space);
        space->generation++;

        // check duplicate coordinates
        if (space->checkDuplicateVertexInSurfaces()) return -1;

        if (processGenerations(space)) return -1;
/*
        Checker::threshold_2 = 10.0;
        Checker::threshold_1 = 40.0;
        if (processGenerations(space)) return -1;
*/
        if (space->checkSurfaceValid() == -1){ cout << "Surface is not valid" << endl; return -1; }
        space->sortSurfacesByArea();
    }
    return 0;
}

int Converter::doValidation() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        PolyhedralSurface *space = this->spaceList[it];
        if (!space->isClosed())
            cerr << "space " << space->name << " is not closed" << endl;
        space->checkSelfIntersection();
    }
    return 0;
}



int Converter::processGenerations(PolyhedralSurface *space) {
    ll p_size = (ull)space->surfacesList.size();
    double thres1 = options.threshold_1;
    double thres2 = options.threshold_2;

    while (true){
        assert(p_size > 0);
        cout << "generation " << space->generation << ": " << space->surfacesList.size()<< endl;
        cout << "degree  : " << thres1 << endl;
        bool hasMerged = TM2IN::algorithm::mergeSurfaces(space, thres1, thres2);

        int simplifySegment = 0;
        if (!hasMerged){
            simplifySegment = TM2IN::algorithm::cleanMergedSurfaces(space); // space->clean_merging_result();
            if (simplifySegment == -1){
                return -1;
            }
        }

        if (space->checkSurfaceValid() == -1){
            cerr << "Surface is not valid" << endl;
            return -1;
        }

        if (hasMerged || simplifySegment){
            p_size = (int)space->surfacesList.size();
        }
        else{
            cout << "generation " << space->generation << " done..\n\n\n"<< endl;
            break;
        }

        if (this->generation_writer) this->generation_writer->write();
        if (thres1 < 40) thres1 += 2.0;

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
