
#include <iostream>
#include <fstream>
#include <string>

#include "fileio/TVRImporter.h"
#include "manage/ManagerImpl.h"
#include "logic/check.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    //TODO make argv
    double degree_param = 10.00;
    string import_version = "_v0.2.6.1";
    string export_version = "_v0.2.6.2";
    const char path[50] = "../Resource/teevr/tvr/";
    //const char path[100] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/";
    const char result_path[50] = "../Result/";
    //const char result_path[50] = "/Users/dong/Documents/dev/TriMeshToGeom/Result/";
    char file_name[] = "main";

//    TVRImporter::extractMINtvr(string(path) + string(file_name));
//    return 0;


    cout << "select mode" << endl;
    cout << "0 : make new Surfaces" << endl;
    cout << "1 : import Surfaces" <<endl;
    cout << "2 : import and onlyJoin" <<endl;
    int mode; cin >> mode;

    Manager* manager = new ManagerImpl();
    manager->setImporter(new TVRImporter());
    manager->setChecker(new Checker(0.000001, degree_param));

    cout << "Load TVR File.." << endl;
    if (manager->import( (string(path) + string(file_name) + ".tvr").c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    string imported_bin = string(result_path) + "cb_" + string(file_name) + import_version + ".bin";
    string exported_bin = string(result_path) + "cb_" + string(file_name) + export_version + ".bin";
    switch(mode){
        case 0:{
            if (manager->makeSurfaces()) return 1;
            if (manager->exportCombined(exported_bin)) return 2;
            break;
        }
        case 1:{
            if (manager->importCombined(imported_bin)) return 3;
            break;
        }
        case 2:{
            if (manager->importCombined(imported_bin)) return 4;
            string json_file = string(result_path) + string(file_name) + "_" + to_string(degree_param).substr(0,4) + export_version + "_onlyJoin" + ".json";
            manager->exportJSON(json_file);
            return 0;
        }
    }

    //Make each surfaces planar and Remove the tilted surface (and remove co-linear).
    if (manager->cleaning() == -1) return -1;

    string json_file = string(result_path) + string(file_name) + "_" + to_string(degree_param).substr(0,4) + export_version + ".json";
    manager->exportJSON(json_file);



    std::cout << "End!\n";
    return 0;
}
