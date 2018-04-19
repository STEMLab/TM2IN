
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <fileio/export/JSONSurfaceExporter.h>
#include "fileio/import/TVRImporter.h"
#include <converter/TriangulationConverter.h>
#include <fileio/import/ThreeDSImporter.h>
#include <fileio/import/COLLADAImporter.h>

void createAndRemoveDir(const string &version, const string &resultPath, const string &fileName);
map<string, string> getPaths(int type);

using namespace std;

int main(int argc, const char * argv[]) {
//    Vector_3 vec1(-0.051682, -0.016531, 0.005755);
//    Vector_3 vec2(-0.000627, -0.000565, 0.000068);
//    cout << CGALCalculation::getAngle(vec1, vec2) << endl;
//
//    Point_3 p1(0,0,0), p2(4,2,0), p3(2,4,0);
//    cout << CGAL::normal(p1, p2, p3) << endl;
//    return 0;
    Checker::thresholdVertex = 0.0000001;
    Checker::squaredDistanceOfSamePoint2D = 0.000001;
    Checker::coplanar_degree = 1.0;
    Checker::merge_degree = 10.0;
    Checker::degreeOfStraight = 0.00001;
    Checker::num_of_straight = 0;
    Checker::num_of_invalid = 0;

    int dataType;
    cout << "choose Data type to import" << endl;
    cout << "1. TVR\n2. 3DS\n3. COLLADA\n";
    cin >> dataType;
    map<string, string> paths = getPaths(dataType);

    string fileName;
    cout << "write file name" << endl;
    cin >> fileName;
    paths["filename"] = fileName;

    string version = "0.4.0"; cout << version << endl;
    paths["version"] = version;
    paths["versionDir"] = paths["resultDir"] + paths["filename"] + "/" + paths["version"] + "/";

    Converter* converter = new TriangulationConverter();
    if (dataType == 1)  converter->setImporter(new TVRImporter());
    else if (dataType == 2) converter->setImporter(new ThreeDSImporter());
    else if (dataType == 3) converter->setImporter(new COLLADAImporter());
    else return -1;

    converter->setGenerationWriter(new GenerationWriter(paths["versionDir"]));
    converter->setExporter(new JSONSurfaceExporter());
    converter->setPaths(paths);

    cout << "Load TM File.." << endl;
    if (converter->importMesh()){
        cout << "Load File Error";
        return -1;
    }

    // create Result directory
    createAndRemoveDir(version, paths["resultDir"], fileName);

    if (converter->pre_process() == -1) return -1;
    if (converter->constructSpace() == -1) return -1;
    if (converter->finish() == -1) return -1;

    char doExport3DS;
    cout <<"Export 3DS?" << endl; cin >>doExport3DS;
    if (checkAnswer(doExport3DS, 'y')){
        converter->export3DS();
    }

    std::cout << "End!\n";
    std::cout << "straight vertex : " << Checker::num_of_straight <<endl;
    std::cout << "invalid Surface : " << Checker::num_of_invalid << endl;
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

void createAndRemoveDir(const string &version, const string &resultPath, const string &fileName) {
    if (boost::filesystem::exists(resultPath + fileName)){
        if (boost::filesystem::exists(resultPath + fileName + "/" + version)){
            char ans;
            cout << "\n\nThis version " << version << " folder exist. Remove Files in directory? (y/n)" << endl;
            cin >> ans;
            if (ans == 'y' || ans == 'Y')
                removeFilesInDirectory(resultPath + fileName + "/" + version);
        }
        else{
            boost::filesystem::create_directory(resultPath + fileName + "/" + version);
        }
    }
    else{
        boost::filesystem::create_directory(resultPath + fileName);
        boost::filesystem::create_directory(resultPath + fileName + "/" + version);
    }
}
