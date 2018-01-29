
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
    Point_2 p5(7.840458, -3.34409);
    Point_2 p6(7.997811, -3.248648);
    Point_2 p7(9.161979, -2.969505);

    Point_2 p42(8.031578, -3.228158);
    Point_2 p43(8.027593, -3.241511);
    Point_2 p44(7.812693, -3.293034);

    Segment_2 seg1(p6 ,p7);
    Segment_2 seg2(p43, p44);

    cout << (Checker::threshold_vertex > CGAL::squared_distance(p43 , seg1)) << endl;

    cout << CGAL::do_intersect(seg1, seg2) << endl;
    if (CGAL::do_intersect(seg1, seg2)){
        CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
                result = CGAL::intersection(seg1, seg2);
        if (result){
            if (const Point_2* p = boost::get<Point_2>(&*result)){
                std::cout << *p << endl;
                cout << (*p == p43) << endl;
            }
            else{
                const Segment_2* sg = boost::get<Segment_2>(&*result);
                cout << sg << endl;
            }
        }
    }
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

    cout << "select mode" << endl;
    cout << "0 : make new Surfaces" << endl;
    cout << "1 : import Surfaces Generation" <<endl;
    cout << "2 : import and onlyJoin" <<endl;
    int mode; cin >> mode;

    RoomMaker* manager = new MergingRoomMaker();
    manager->setImporter(new TVRImporter());
    Checker::threshold_vertex = 0.0000001;

    cout << "Load TVR File.." << endl;
    if (manager->importMesh( (string(resourcePath) + fileName + ".tvr").c_str()) ){
        cout << "Load File Error";
        return -1;
    }

    // create Result directory
    createAndRemoveDir(version, resultPath, fileName);

    manager->setGenerationWriter(new GenerationWriter(generationWritePath));

    switch(mode){
        case 0:{
            if (manager->convertTriangleMeshToSpace(0.0)) return 1;
            break;
        }
        case 1:{
            // TODO
            return 0;
        }
        case 2:{
            if (manager->convertTriangleMeshToSpace(0.0)) return 1;
            string json_file = string(resultPath) + fileName + "/" + version + "/" + "triangles.json";
            manager->exportSpaceJSON(json_file);
            return 0;
        }
        default:{
            cout << "no Mode" << endl;
            return 0;
        }
    }

    if (manager->constructSpace() == -1) return -1;

    string surfaceJSON = string(resultPath) + fileName + "/" + version + "/" + "surfaces.json";
    manager->exportSpaceJSON(surfaceJSON);

    string triangulationJSON = string(resultPath) + fileName + "/" + version + "/" + "triangles.json";
    manager->exportTriangulationJSON(triangulationJSON);

    cout << "make solid?(y or n)" << endl;
    char ans_simple; cin >> ans_simple;
    if (ans_simple == 'y'){
        if (manager->finish() == -1) return -1;
        string simple_file = string(resultPath) + fileName + version + "_solid.json";
        manager->exportSpaceJSON(simple_file);
    }


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
