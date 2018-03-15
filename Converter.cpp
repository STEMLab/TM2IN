
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <fileio/export/JSONSurfaceExporter.h>
#include "fileio/import/TVRImporter.h"
#include <manage/MergingRoomMaker.h>
#include <fileio/import/ThreeDSImporter.h>

void createAndRemoveDir(const string &version, const string &resultPath, const string &fileName);
map<string, string> getPaths(int type);

using namespace std;

void test(){
    Point_3 p51(-373.2655, 360.9103, 49.3925);
    Point_3 p52(-372.7594, 373.1839, 25.1797);
    Point_3 p0(-373.2519, 361.2404, 48.7411);
    Point_3 p1(-374.2917, 359.6632, 42.686);

    Segment_3 seg1(p51 ,p52);
    Segment_3 seg2(p0, p1);

    cout << CGAL::squared_distance(seg1, p0) << endl;
    cout << CGAL::do_intersect(seg1, seg2) << endl;

}

int main(int argc, const char * argv[]) {
    string version = "0.3.1";
    cout << version << endl;

    RoomMaker* manager = new MergingRoomMaker();

    cout << "choose Data type to import" << endl;
    cout << "1. TVR\n2. 3DS\n3. COLLADA\n";
    int dataType; cin >> dataType;

    map<string, string> paths = getPaths(dataType);

    string fileName;
    cout << "write file name" << endl; cin >> fileName;

    const int maxGENperOneCycle = 20;
    string generationWritePath = paths["result"] + fileName+"/" + version + "/";

    manager->setImporter(new ThreeDSImporter());
    Checker::threshold_vertex = 0.0000001;
    Checker::squaredDistanceOfSamePoint2D = 0.000001;

    cout << "Load TVR File.." << endl;
    if (manager->importMesh( (string(paths["resource"]) + fileName + "." + paths["filetype"]).c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    // create Result directory
    createAndRemoveDir(version, paths["result"], fileName);
    manager->setGenerationWriter(new GenerationWriter(generationWritePath));

    if (manager->pre_process() == -1) return -1;
    if (manager->constructSpace() == -1) return -1;

    manager->setExporter(new JSONSurfaceExporter());
    manager->exportSpace( (string(paths["result"]) + fileName + "/" + version + "/surfaces.json").c_str());

//    string surfaceJSON = string(paths["result"]) + fileName + "/" + version + "/" + "surfaces.json";
//    manager->exportSpaceJSON(surfaceJSON);
//
//    string triangulationJSON = string(resultPath) + fileName + "/" + version + "/" + "triangles.json";
//    manager->exportTriangulationJSON(triangulationJSON);

    std::cout << "End!\n";
    return 0;
}

map<string, string> getPaths(int type){
    map<string, string> paths;

    switch(type){
        case 1: //TVR
            paths["project"] = "/home/dongmin/dev/TriMeshToIndoor/";
            paths["resource"] = paths["project"] + "Resource/tvr/";
            paths["result"] = paths["project"] + "Result/";
            paths["filetype"] = "tvr";
            break;
        case 2: //3DS
            paths["project"] = "/home/dongmin/dev/TriMeshToIndoor/";
            paths["resource"] = paths["project"] + "Resource/3DS/";
            paths["result"] = paths["project"] + "Result/";
            paths["filetype"] = "3DS";
            break;
        case 3: //COLLADA
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
