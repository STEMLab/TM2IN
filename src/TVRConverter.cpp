
#include <iostream>
#include <fstream>
#include <string>

#include "fileio/TVRImporter.h"
#include "manage/FlatRoomMaker.h"
#include "logic/check.hpp"
#include "space_maker/OnlyWallSpaceMaker.h"

using namespace std;

void test(){
    Point_3 p1(-0.514636, 1.815419, -1.287682);
    Point_3 p2(-0.505437, 1.807147, -1.288355);
    Point_3 p3(-0.505941, 1.807601, -1.288318);

    Vector_3 vc1(p1,p2);
    Vector_3 vc2(p1,p3);
    cout << CGALCalculation::getAngle(vc1, vc2) << endl;
}

int main(int argc, const char * argv[]) {
    //test(); return 0;

    string import_version = "1.0.1.2";
    string export_version = "1.0.1.2";

    /**
    * for Mac
    */
    //const char path[100] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/";
    //const char result_path[50] = "/Users/dong/Documents/dev/TriMeshToGeom/Result/";

    const char path[50] = "../Resource/tvr/";
    const char result_path[50] = "../Result/";
    string file_name;
    cout << "write file name" << endl;
    cin >> file_name;
    //char file_name[] = "171103_APT";
    const int maxGENperOneCycle = 20;
    string generation_write_path = "../Result/"+file_name+"/" + export_version + "/";

//    TVRImporter::extractMINtvr(string(path) + string(file_name));
//    return 0;

    cout << "select mode" << endl;
    cout << "0 : make new Surfaces" << endl;
    cout << "1 : import Surfaces Generation" <<endl;
    cout << "2 : import and onlyJoin" <<endl;
    int mode; cin >> mode;

    RoomMaker* manager = new FlatRoomMaker();
    manager->setImporter(new TVRImporter());
    manager->setChecker(new Checker(0.0000001));

    cout << "Load TVR File.." << endl;
    if (manager->import( (string(path) + file_name + ".tvr").c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    // TODO
    // mkdir version

    manager->setGenerationWriter(new GenerationWriter(generation_write_path));

    switch(mode){
        case 0:{
            if (manager->pre_process(0.0)) return 1;
            //if (manager->exportCombined(exported_bin)) return 2;
            break;
        }
        case 1:{

            cout << "choose Generation" << endl;
            // TODO
            /*
            int gen; cin >> gen;
            string import_path = string(result_path) + file_name + "/" + import_version + "/generation_" + gen;
            if (manager->importGeneration(import_path)) return 3;
            */
            break;
        }
        case 2:{
            // TODO
            /*
            if (manager->importGeneration(imported_bin)) return 4;
            string json_file = string(result_path) + file_name + export_version + "_onlyJoin" + ".json";
            manager->exportSpaceJSON(json_file);
            */
            return 0;
        }
        default:{
            cout << "no Mode" << endl;
            return 0;
        }
    }

    cout << "simplify line?(y or n)" << endl;
    char ans_simple_line; cin >> ans_simple_line;
    bool simplifyLine = ans_simple_line == 'y';

    cout << "snap Surface?(y or n)" << endl;
    char ans_snap_surface; cin >> ans_snap_surface;
    bool snapSurface = ans_snap_surface == 'y';

    if (manager->constructSpace(new CombineParameter(maxGENperOneCycle, 1.00, simplifyLine, snapSurface)) == -1) return -1;

    string json_file = string(result_path) + string(file_name) + "/" + export_version + "/" + "surfaces.json";
    manager->exportSpaceJSON(json_file);

    cout << "do simple?(y or n)" << endl;
    char ans_simple; cin >> ans_simple;
    if (ans_simple == 'y'){
        if (manager->makeSimpleSpaces(new OnlyWallSpaceMaker()) == -1) return -1;
        string simple_file = string(result_path) + string(file_name) + export_version + "_solid.json";
        manager->exportSpaceJSON(simple_file);
    }


    std::cout << "End!\n";
    return 0;
}
