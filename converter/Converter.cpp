#include <io/tvr.h>
#include <io/max3ds.h>
#include <io/collada.h>
#include <io/json.h>
#include <algorithm/mbb.h>

#include "config.h"
#include "features/HalfEdge.h"
#include "features/Wall/Triangle.h"
#include "features/Room.h"
#include "features/RoomBoundary/TriangleMesh.h"
#include "features/RoomBoundary/PolygonMesh.h"

#include "Converter.h"

using namespace TM2IN;

Converter::Converter(){}

int Converter::start() {
    importData(); // import mesh data
    /*
    if (options.generator) generation_writer = new GenerationWriter(options.output_dir); // generation writer
    */
    return 0;
}

int Converter::run() {
    mergeSurfaces();
    if (Options::getInstance()->do_validation)
        validate_tsm();

    if (Options::getInstance()->polygonizer_mode > 0) // 1 or 2 or 3
        polygonize();

    return 0;
}

int Converter::finish() {
    this->tag_pm_ID();
    this->exportRoomBoundary();

    return 0;
}

int Converter::exportRoomBoundary() {
    //JSON
    TM2IN::io::exportRoomBoundaryJSON(Options::getInstance()->output_dir + "surfaces.json", this->rooms, false);

    if (Options::getInstance()->output_3ds || Options::getInstance()->output_tvr){
        convert_pm_to_tm();
    }

    //TVR
    if (Options::getInstance()->output_tvr){

    }

    //3DS
    if (Options::getInstance()->output_3ds){
        TM2IN::io::export3DS((Options::getInstance()->output_dir + Options::getInstance()->file_name + ".3DS").c_str(), this->rooms);
    }
    return 0;
}

int Converter::convert_pm_to_tm(){
    for (int room_id = 0 ; room_id < this->rooms.size() ; room_id++){
        TM2IN::Room* room = this->rooms[room_id];

        RoomBoundary::TriangleMesh* tm = room->getPm_boundary()->to_triangle_mesh();
        room->setTm_boundary(tm);
    }
    return 0;
}


void Converter::tag_pm_ID() {
    for (ull it = 0 ; it < this->rooms.size() ; it++) {
        Room *room = this->rooms[it];
        room->getPm_boundary()->tag_ID(room->name);
    }
}

/**
 * @todo Implementation
 * @return
 */
int Converter::handleOpenTriangleMesh() {
    throw std::runtime_error("TODO : handleOpenTriangleMesh");
    // printf("\n\n%d meshes have been removed because it is open.\n", count);
}
