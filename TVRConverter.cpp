
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <string>

#include "fileio/TVRImporter.h"
#include "manage/MergingRoomMaker.h"
#include "logic/check.hpp"
#include "space_maker/OnlyWallSpaceMaker.h"

void createAndRemoveDir(const string &version, const string &resultPath, const string &fileName);

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
//    test();
//    return 0;
    string version = "1.0.1.5";
    cout << version << endl;

    string projectPath = "/home/dongmin/dev/TriMeshToIndoor/";
    string resourcePath = projectPath + "Resource/tvr/";
    string resultPath = projectPath + "Result/";
    string fileName;
    cout << "write file name" << endl;
    cin >> fileName;
    const int maxGENperOneCycle = 20;
    string generationWritePath = resultPath + fileName+"/" + version + "/";

//    TVRImporter::extractMINtvr(string(path) + string(fileName));
//    return 0;

    RoomMaker* manager = new MergingRoomMaker();
    manager->setImporter(new TVRImporter());
    Checker::threshold_vertex = 0.0000001;
    Checker::squaredDistanceOfSamePoint2D = 0.000001;

    cout << "Load TVR File.." << endl;
    if (manager->importMesh( (string(resourcePath) + fileName + ".tvr").c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    // create Result directory
    createAndRemoveDir(version, resultPath, fileName);
    manager->setGenerationWriter(new GenerationWriter(generationWritePath));

    if (manager->pre_process() == -1) return -1;
    if (manager->constructSpace() == -1) return -1;

    string surfaceJSON = string(resultPath) + fileName + "/" + version + "/" + "surfaces.json";
    manager->exportSpaceJSON(surfaceJSON);

    string triangulationJSON = string(resultPath) + fileName + "/" + version + "/" + "triangles.json";
    manager->exportTriangulationJSON(triangulationJSON);

    /*
    cout << "make solid?(y or n)" << endl;
    char ans_simple; cin >> ans_simple;
    if (ans_simple == 'y'){
        if (manager->finish() == -1) return -1;
        string simple_file = string(resultPath) + fileName + version + "_solid.json";
        manager->exportSpaceJSON(simple_file);
    }

*/
    std::cout << "End!\n";
    return 0;
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
