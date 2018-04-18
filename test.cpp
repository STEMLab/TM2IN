//
// Created by dongmin on 18. 4. 17.
//

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <fileio/export/JSONSurfaceExporter.h>
#include "fileio/import/TVRImporter.h"
#include <converter/TriangulationConverter.h>
#include <fileio/import/ThreeDSImporter.h>
#include <fileio/import/COLLADAImporter.h>
map<string, string> getPaths(int type);

using namespace std;

int main(int argc, const char * argv[]) {
    Checker::thresholdVertex = 0.0000001;
    Checker::squaredDistanceOfSamePoint2D = 0.000001;
    Checker::degreeOfMerging = 1.0;
    Checker::degreeOfStraight = 0.00001;
    Checker::num_of_straight = 0;
    Checker::num_of_invalid = 0;

    int dataType = 1; // cout << "1. TVR\n2. 3DS\n3. COLLADA\n";
    map<string, string> paths = getPaths(dataType);

    string fileName("apt3");
    paths["filename"] = fileName;

    string version = "test";
    cout << version << endl;
    paths["version"] = version;
    paths["versionDir"] = paths["resultDir"] + paths["filename"] + "/" + paths["version"] + "/";

    Converter* manager = new TriangulationConverter();
    if (dataType == 1)  manager->setImporter(new TVRImporter());
    else if (dataType == 2) manager->setImporter(new ThreeDSImporter());
    else if (dataType == 3) manager->setImporter(new COLLADAImporter());

    manager->setExporter(new JSONSurfaceExporter());
    manager->setPaths(paths);

    cout << "Load TM File.." << endl;
    if (manager->importMesh()){
        cout << "Load File Error";
        return -1;
    }

    if (manager->pre_process() == -1) return -1;
    if (manager->constructSpace() == -1) return -1;
    if (manager->finish() == -1) return -1;

    cout << CGALCalculation::getAngle(manager->spaceList[0]->surfacesList[98]->normal,manager->spaceList[0]->surfacesList[103]->normal) << endl;
    std::cout << "End!\n";
    return 0;
}

map<string, string> getPaths(int type){
    map<string, string> paths;

    switch(type){
        case 1: //TVR
            paths["projectDir"] = "/home/dongmin/dev/TriMeshToIndoor/";
            paths["resourceDir"] = paths["projectDir"] + "Resource/tvr/";
            paths["resultDir"] = paths["projectDir"] + "Result/";
            paths["filetype"] = "tvr";
            break;
        case 2: //3DS
            paths["projectDir"] = "/home/dongmin/dev/TriMeshToIndoor/";
            paths["resourceDir"] = paths["projectDir"] + "Resource/3DS/";
            paths["resultDir"] = paths["projectDir"] + "Result/";
            paths["filetype"] = "3DS";
            break;
        case 3: //COLLADA
            paths["projectDir"] = "/home/dongmin/dev/TriMeshToIndoor/";
            paths["resourceDir"] = paths["projectDir"] + "Resource/COLLADA/";
            paths["resultDir"] = paths["projectDir"] + "Result/";
            paths["filetype"] = "dae";
            break;
        default:
            exit(-1);
    }

    return paths;
}
