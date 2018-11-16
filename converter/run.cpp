//
// Created by dongmin on 18. 7. 19.
//
#include "converter/Converter.h"

#include <algorithm/merge_surfaces.h>
#include "io/GenerationWriter.h"
#include "features/Room.h"
#include "features/RoomBoundary/TriangulatedSurfaceMesh.h"


int Converter::mergeSurfaces() {
    TM2IN::io::GenerationWriter* gw = TM2IN::io::GenerationWriter::getInstance();
    clock_t begin = clock();
    for (ull it = 0 ; it < this->rooms.size(); it++)
    {
        Room* room = this->rooms[it];
        if (Options::getInstance()->generator)
            gw->start(room);
        make_tri_surface_mesh(room);
        if (Options::getInstance()->generator)
            gw->close();
    }
    clock_t end = clock();
    cout << "merge Surfaces time : " << double(end - begin) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}

void Converter::make_tri_surface_mesh(Room* room) {
    RoomBoundary::TriangleMesh *tm = room->getTm_boundary();
    RoomBoundary::TriangulatedSurfaceMesh* tsm = new RoomBoundary::TriangulatedSurfaceMesh(tm);
    room->setTsm_boundary(tsm);

    ll p_size = (ull)tsm->num_of_surfaces();
    double thres1 = Options::getInstance()->threshold_1;
    double thres2 = Options::getInstance()->threshold_2;
    Options::getInstance()->generation = 0;

    if (Options::getInstance()->has_no_merge)
        return;

    while (true){
        if (Options::getInstance()->generator)
            TM2IN::io::GenerationWriter::getInstance()->write(tsm);
        assert(p_size > 0);
        cout << "generation " << Options::getInstance()->generation << ": " << tsm->num_of_surfaces()<< endl;
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
            cout << "generation " << Options::getInstance()->generation << " done..\n\n\n"<< endl;
            break;
        }
        if (thres1 < 40) thres1 += 2.0;

        Options::getInstance()->generation = Options::getInstance()->generation + 1;
        // tsm->update_surfaces_normal();
    }

}

int Converter::validate_tsm() {
    for (ull it = 0 ; it < this->rooms.size() ; it++) {
        auto room = this->rooms[it];
        RoomBoundary::TriangulatedSurfaceMesh* tsm = room->getTsm_boundary();
        if (!tsm->isClosed()){
            throw std::runtime_error("space " + room->geom_id + " is not closed");
        }
        if (tsm->surface_strict_validation()){
            throw std::runtime_error("space " + room->geom_id + "'s surfaces are not valid");
        }
    }
    return 0;
}

int Converter::classify_sf_type(){
    for (ull it = 0 ; it < this->rooms.size() ; it++) {
        auto room = this->rooms[it];
        RoomBoundary::TriangulatedSurfaceMesh* tsm = room->getTsm_boundary();
        tsm->classify_sf_type();
    }
    return 0;
}

int Converter::polygonize() {
    TM2IN::algorithm::Polygonizer* polygonizer = create_polygonizer();
    for (ull it = 0 ; it < this->rooms.size() ; it++) {
        auto room = this->rooms[it];
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

