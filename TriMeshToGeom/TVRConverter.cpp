
#include <iostream>
#include <fstream>
#include <string>

#include "ObjectCollection.h"
#include "TVRImporter.h"
#include "ManagerImpl.h"
#include "Manager.h"
#include "check.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
//TODO make argv
    double degree_param = 10.0;
    string version = "_v0.2.4";
    const char path[50] = "../Resource/teevr/tvr/";
    //const char path[100] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/";
    const char result_path[50] = "../Result/";
    //const char result_path[50] = "/Users/dong/Documents/dev/TriMeshToGeom/Result/";
    char file_name[] = "cube_01";


    Manager* manager = new ManagerImpl();
    manager->setImporter(new TVRImporter());
    manager->setChecker(new Checker(0.0000001, degree_param));

    cout << "Load TVR File.." << endl;
    if (manager->import( (string(path) + string(file_name) + ".tvr").c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    string combined_file = string(result_path) + "c_" + string(file_name) + version + ".bin";

    cout << "select mode" << endl;
    cout << "0 : make new Surfaces" << endl;
    cout << "1 : import Surfaces" <<endl;
    int mode; cin >> mode;
    switch(mode){
        case 0:{
            if (manager->makeSurfaces()) return 1;
            if (manager->exportCombined(combined_file)) return 2;
            break;
        }
        case 1:{
            if (manager->importCombined(combined_file)) return 3;
            break;
        }
    }

    //Make each surfaces planar and Remove the tilted surface (and remove co-linear).
    if (manager->cleaning() == -1) return -1;


    //TODO Pinning
    //TODO Make Solid

    string json_file = string(result_path) + string(file_name) + "_" + to_string(degree_param).substr(0,4) + version + ".json";
    manager->exportJSON(json_file);



    std::cout << "End!\n";
    return 0;
}
