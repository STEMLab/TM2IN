//
// Created by dongmin on 18. 7. 19.
//
#include "converter/Converter.h"

#include <algorithm/merge_surfaces.h>
#include "features/Room.h"
#include "features/RoomBoundary/TriangulatedSurfaceMesh.h"


int Converter::mergeSurfaces() {
    clock_t begin = clock();
    for (ull it = 0 ; it < this->rooms.size(); it++)
    {
        Room* room = this->rooms[it];

        RoomBoundary::TriangulatedSurfaceMesh* tsm = make_tri_surface_mesh(room->getTm_boundary());
        room->setTsm_boundary(tsm);
    }
    clock_t end = clock();
    cout << "merge Surfaces time : " << double(end - begin) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}

RoomBoundary::TriangulatedSurfaceMesh *Converter::make_tri_surface_mesh(RoomBoundary::TriangleMesh *tm) {
    RoomBoundary::TriangulatedSurfaceMesh* tsm = new RoomBoundary::TriangulatedSurfaceMesh(tm);
    ll p_size = (ull)tsm->num_of_surfaces();
    double thres1 = Options::getInstance()->threshold_1;
    double thres2 = Options::getInstance()->threshold_2;
    int generation = 0;

    while (true){
        assert(p_size > 0);
        cout << "generation " << generation << ": " << tsm->num_of_surfaces()<< endl;
        cout << "degree  : " << thres1 << endl;
        bool hasMerged = TM2IN::algorithm::mergeSurfaces(tsm, thres1, thres2);

        int simplifySegment = 0;
        if (!hasMerged){
            simplifySegment = TM2IN::algorithm::cleanMergedSurfaces(tsm); // space->clean_merging_result();
            if (simplifySegment == -1){
                throw std::runtime_error("simplifySegment\n");
            }
        }

        if (tsm->surface_easy_validation() == -1){
            throw std::runtime_error("Surface is not valid\n");
        }

        if (hasMerged || simplifySegment){
            p_size = (int)tsm->num_of_surfaces();
        }
        else{
            cout << "generation " << generation << " done..\n\n\n"<< endl;
            break;
        }
        if (thres1 < 40) thres1 += 2.0;

        generation++;
        tsm->update_surfaces_normal();
    }

    return tsm;
}

int Converter::validate_tsm() {
    for (ull it = 0 ; it < this->rooms.size() ; it++) {
        TM2IN::Room *room = this->rooms[it];
        RoomBoundary::TriangulatedSurfaceMesh* tsm = room->getTsm_boundary();
        if (!tsm->isClosed()){
            throw std::runtime_error("space " + room->name + " is not closed");
        }
        if (tsm->surface_strict_validation()){
            throw std::runtime_error("space " + room->name + "'s surfaces are not valid");
        }
    }
    return 0;
}



int Converter::polygonize() {
    TM2IN::algorithm::Polygonizer* polygonizer = create_polygonizer();
    for (ull it = 0 ; it < this->rooms.size() ; it++) {
        Room* room = this->rooms[it];
        RoomBoundary::PolygonMesh* pm = polygonizer->run(room->getTsm_boundary());
        room->setPm_boundary(pm);
    }
    return 0;
}


TM2IN::algorithm::Polygonizer *Converter::create_polygonizer() {
    switch(Options::getInstance()->polygonizer_mode){
        case 1:
            return new TM2IN::algorithm::PCAPolygonizer();
        case 2:
            return new TM2IN::algorithm::TrianglePolygonizer();
        case 3:
            return new TM2IN::algorithm::DividedPolygonizer();
        default:
            break;
    }
    throw std::runtime_error("options.polygonizer_mode has wrong value");
}

