
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <fileio/export/JSONSurfaceExporter.h>
#include "fileio/import/TVRImporter.h"
#include <command/Merge_Command.h>
#include <fileio/import/ThreeDSImporter.h>
#include <fileio/import/COLLADAImporter.h>

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
    Checker::thresholdVertex = 0.0000001;
    Checker::squaredDistanceOfSamePoint2D = 0.000001;
    Checker::degreeOfMerging = 10.0;
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

    string version = "0.3.9"; cout << version << endl;
    paths["version"] = version;
    paths["versionDir"] = paths["resultDir"] + paths["filename"] + "/" + paths["version"] + "/";

    Command* manager = new Merge_Command();
    if (dataType == 1)  manager->setImporter(new TVRImporter());
    else if (dataType == 2) manager->setImporter(new ThreeDSImporter());
    else if (dataType == 3) manager->setImporter(new COLLADAImporter());
    else return -1;

    manager->setGenerationWriter(new GenerationWriter(paths["versionDir"]));
    manager->setExporter(new JSONSurfaceExporter());
    manager->setPaths(paths);

    cout << "Load TM File.." << endl;
    if (manager->importMesh()){
        cout << "Load File Error";
        return -1;
    }

    // create Result directory
    createAndRemoveDir(version, paths["resultDir"], fileName);

    if (manager->pre_process() == -1) return -1;
    if (manager->constructSpace() == -1) return -1;
    if (manager->finish() == -1) return -1;

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
