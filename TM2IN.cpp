
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <polygonizer/TrianglePolygonizer.h>
#include <polygonizer/DividedPolygonizer.h>
#include <converter/Converter.h>
#include <polygonizer/PCAPolygonizer.h>
#include "predefine.h"

void createAndRemoveDir(const string &version, const string &resultPath, const string &fileName);
map<string, string> getPaths(int type);

using namespace std;

int main(int argc, const char * argv[]) {
    Checker::threshold_same_vt_distance = 0.0000001;
    Checker::threshold_1 = 1.0;
    Checker::threshold_2 = 10.0;
    Checker::threshold_collinear = 0.00001;
    Checker::num_of_straight = 0;
    Checker::num_of_invalid = 0;

    cout << "Input File Path" << endl;
    string input_file;
    if (argc < 2){
        cin >> input_file;
    }
    else
        input_file = argv[1];

    cout << "Result Directory" << endl;
    string result_dir;
    if (argc < 3){
        cin >> result_dir;
    } else
        result_dir = argv[2];

    cout << "Version" << endl;
    string version;
    if (argc < 4)
        cin >> version;
    else
        version = argv[3];


//    map<string, string> paths = getPaths(dataType);
//    cout << "write file name" << endl;
//    string dataName; cin >> dataName; // apt3
//    paths["dataName"] = dataName;
//    paths["outputDataName"] = dataName + ".json";
//
//    cout << "version" << endl;
//    string version; cin >> version; // x.y.z
//    cout << version << endl;
//    paths["version"] = version;
//    paths["versionDir"] = paths["resultDir"] + paths["dataName"] + "/" + paths["version"] + "/";

    Converter* converter = new Converter();
    if (dataType == 1)  converter->importTVR(new TVRImporter());
    else if (dataType == 2) converter->import3DS(new ThreeDSImporter());
    else if (dataType == 3) converter->importCOLLADA(new COLLADAImporter());
    else return -1;

    converter->setGenerationWriter(new GenerationWriter(paths["versionDir"]));



    converter->setExporter(new SpaceExporter());
    converter->setPaths(paths);

    cout << "Load TM File.." << endl;
    if (converter->importMesh()){
        cout << "Load File Error";
        return -1;
    } else
        converter->printInputDataSpec();

    // create Result directory
    createAndRemoveDir(version, paths["resultDir"], dataName);

    cout << "PRE PROCESS" << endl;
    if (converter->initTriangleMesh()) return -1;
    if (converter->handleOpenTriangleMesh()) return -1;
    if (converter->partitionTriangleMeshByComponent()) return -1;
    if (converter->remainSelectedMesh(ARCH)) return -1;
    if (converter->convertTriangleMeshToSpace()) return -1;

    cout << "CONSTRUCT SPACE" << endl;
    assert (converter->spaceList.size() != 0);

    if (converter->mergeSurfaces()) return -1;
    // if (this->simplifyShareEdge()) return -1;
    converter->makeSurfaceGraph();
    converter->checkSelfIntersection();


    cout << "TRIANGULATION" << endl;
    // if (converter->triangulation()) return -1;

    cout << "POLYGONIZER" << endl;
    // Polygonizer* polygonizer = new DividedPolygonizer();
    // Polygonizer* polygonizer = new PCAPolygonizer();
    Polygonizer* polygonizer = NULL;
    if (converter->polygonize(polygonizer) == -1) return -1;

    cout << "FINISH" << endl;
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
