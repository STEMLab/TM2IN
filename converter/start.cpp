//
// Created by dongmin on 18. 7. 18.
//


#include <algorithm/mbb.h>
#include "converter/Converter.h"

#include "io/tvr.h"
#include "io/collada.h"
#include "io/max3ds.h"
#include "features/Wall/Triangle.h"
#include "features/Room.h"
#include "features/RoomBoundary/TriangleMesh.h"

int Converter::importData() {
    switch(Options::getInstance()->input_type){
        case 1:
            this->rooms = TM2IN::io::importTVR((Options::getInstance()->input_dir + Options::getInstance()->input_file).c_str());
            break;
        case 2:
            this->rooms = TM2IN::io::import3DS((Options::getInstance()->input_dir + Options::getInstance()->input_file).c_str());
            break;
        case 3:
            this->rooms = TM2IN::io::importDAE((Options::getInstance()->input_dir + Options::getInstance()->input_file).c_str());
            break;
        default:
            throw std::runtime_error("\n\nImport Mesh has some problem\n\n");
    }
    if (this->rooms.size() == 0){
        throw std::runtime_error("\n\nImport Mesh has some problem\n\n");
    }
    print_input_spec();
    return 0;
}


/**
 * Print Data speciation
 */
void Converter::print_input_spec() {
    double area_sum = 0.0;
    vector<Wall::Surface*> surfaces;
    for (ull it = 0 ; it < this->rooms.size() ; it++){
        for (Wall::Triangle* triangle : this->rooms[it]->getTm_boundary()->getTriangleList()){
            surfaces.push_back((Wall::Surface*)triangle);
            area_sum += triangle->getArea();
        }
    }

    int trianglesCount = surfaces.size();
    CGAL::Bbox_3 mbb;
    mbb = TM2IN::algorithm::getMBB(surfaces);

    cout << "\n\nTriangles : " << trianglesCount << endl;
    cout << "Bbox : " << mbb << endl;
    cout << "Area : " << area_sum / (double)trianglesCount << endl;
    cout << "\n\n" << endl;
}
