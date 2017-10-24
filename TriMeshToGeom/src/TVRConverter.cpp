
#include <iostream>
#include <fstream>
#include <string>

#include "fileio/TVRImporter.h"
#include "manage/FlatRoomMaker.h"
#include "logic/check.hpp"

using namespace std;

void test(){
//    Vector_3 a = Vector_3(1,1,1);
    Vector_3 b = Vector_3(0,0,-1);
//    cout << CGALCalculation::getAngle(a, b) <<endl;

    Point_3 p1 = Point_3(0,0,0);
    Point_3 p2 = Point_3(0,-3,2);
    Point_3 p3 = Point_3(3,-1,1);

    Plane_3 plane1 = Plane_3(Point_3(0,0,1), b);

    cout << plane1.to_2d(p1) << endl;
    cout << plane1.to_2d(p2) << endl;
    cout << plane1.to_2d(p3) << endl;

    Plane_3 plane2 = Plane_3(Point_3(0,0,1), -b);
    cout << plane2.to_2d(p1) << endl;
    cout << plane2.to_2d(p2) << endl;
    cout << plane2.to_2d(p3) << endl;

}

int main(int argc, const char * argv[]) {
    //TODO make argv
    string import_version = "_v0.2.9";
    string export_version = "_v0.2.9.1";
    const char path[50] = "../Resource/tvr/";
    //const char path[100] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/";
    const char result_path[50] = "../Result/";
    //const char result_path[50] = "/Users/dong/Documents/dev/TriMeshToGeom/Result/";
    char file_name[] = "office";
    const int max_genereration = 2;
    const char process_path[50] = "../Result/process/";
//    TVRImporter::extractMINtvr(string(path) + string(file_name));
//    return 0;

    cout << "select mode" << endl;
    cout << "0 : make new Surfaces" << endl;
    cout << "1 : import Surfaces" <<endl;
    cout << "2 : import and onlyJoin" <<endl;
    int mode; cin >> mode;

    RoomMaker* manager = new FlatRoomMaker();
    manager->setImporter(new TVRImporter());
    manager->setChecker(new Checker(0.000001));

    cout << "Load TVR File.." << endl;
    if (manager->import( (string(path) + string(file_name) + ".tvr").c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    manager->setProcessWriter(new ProcessWriter(string(process_path), string(file_name)));

    string imported_bin = string(result_path) + "polygons/" + string(file_name) + import_version + ".bin";
    string exported_bin = string(result_path) + "polygons/" + string(file_name) + import_version + ".bin";
    switch(mode){
        case 0:{
            if (manager->makeSurfaces(10.0)) return 1;
            if (manager->exportCombined(exported_bin)) return 2;
            break;
        }
        case 1:{
            if (manager->importCombined(imported_bin)) return 3;
            break;
        }
        case 2:{
            if (manager->importCombined(imported_bin)) return 4;
            string json_file = string(result_path) + string(file_name) + export_version + "_onlyJoin" + ".json";
            manager->exportJSON(json_file);
            return 0;
        }
        default:{
            cout << "no Mode" << endl;
            return 0;
        }
    }

    //Make each surfaces planar and Remove the tilted surface (and remove co-linear).
    if (manager->cleaning(max_genereration, 1.00) == -1) return -1;

    string json_file = string(result_path) + string(file_name) + export_version + ".json";
    manager->exportJSON(json_file);

    if (manager->makeSolids(10.0) == -1) return -1;
    string solid_file = string(result_path) + string(file_name) + export_version + "_solid.json";
    manager->exportJSON(solid_file);

    std::cout << "End!\n";
    return 0;
}
