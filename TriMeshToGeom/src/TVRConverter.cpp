
#include <iostream>
#include <fstream>
#include <string>

#include "fileio/TVRImporter.h"
#include "manage/FlatRoomMaker.h"
#include "logic/check.hpp"
#include "space_maker/OnlyWallSpaceMaker.h"

using namespace std;

void test(){
    Point_3 p1(-3.064549, -5.711147, 1.013514);
    Point_3 p2(-3.074714, -5.721137, 0.854335);
    Point_3 p3(-3.026292, -5.738762, 0.772338);

    Vector_3 vc1(p1,p2);
    Vector_3 vc2(p1,p3);
    cout << CGALCalculation::getAngle(vc1, vc2) << endl;
}

int main(int argc, const char * argv[]) {
   // test(); return 0;

    string import_version = "_v1.0.1";
    string export_version = "_v1.0.1.1";
    const char path[50] = "../Resource/tvr/";
    //const char path[100] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/";
    const char result_path[50] = "../Result/";
    //const char result_path[50] = "/Users/dong/Documents/dev/TriMeshToGeom/Result/";
    char file_name[] = "171103_APT";
    const int max_genereration = 10;
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
    manager->setChecker(new Checker(0.0000001));

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
            if (manager->pre_process(0.0)) return 1;
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
            manager->exportSpaceJSON(json_file);
            return 0;
        }
        default:{
            cout << "no Mode" << endl;
            return 0;
        }
    }

    //Make each surfaces planar and Remove the tilted surface (and remove co-linear).
    if (manager->constructSpace(max_genereration, 1.00) == -1) return -1;

    string json_file = string(result_path) + string(file_name) + export_version + ".json";
    manager->exportSpaceJSON(json_file);

//    if (manager->makeSimpleSpaces(new OnlyWallSpaceMaker()) == -1) return -1;
//    string solid_file = string(result_path) + string(file_name) + export_version + "_solid.json";
//    manager->exportSimpleSpaceJSON(solid_file);

    std::cout << "End!\n";
    return 0;
}
