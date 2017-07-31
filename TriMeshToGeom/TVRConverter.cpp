
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
    string version = "_v0.2.2";
    //const char path[50] = "../Resource/teevr/tvr/";
    const char path[100] = "/Users/dong/Documents/dev/TriMeshToGeom/Resource/teevr/tvr/";
    const char result_path[50] = "/Users/dong/Documents/dev/TriMeshToGeom/Result/";
    char file_name[] = "test";

    Manager* manager = new ManagerImpl();
    manager->setImporter(new TVRImporter());
    manager->setChecker(new Checker(0.0000001, degree_param));

    cout << "Load TVR File.." << endl;
    if (manager->import( (string(path) + string(file_name) + ".tvr").c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    manager->makeSurfaces();

    //Make each surfaces planar and Remove the tilted surface (and remove co-linear).
    manager->cleaning();


    //TODO Pinning
    //TODO Make Solid

    string json_file = string(result_path) + string(file_name) + "_" + to_string(degree_param).substr(0,4) + version + ".json";
    manager->exportJSON(json_file);



    std::cout << "End!\n";
    return 0;
}
