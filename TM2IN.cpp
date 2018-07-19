
#include "converter/Converter.h"
#include "Options.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <map>


void createAndRemoveDir(const string &version, const string &resultPath, const string &fileName);
map<string, string> getPaths(int type);

using namespace std;

int main(int argc, char * argv[]) {
    try{
        Options options = Options(argc, argv);
        // createAndRemoveDir(options.version, options.output_dir);
        Converter converter = Converter(options);
        converter.start();

        converter.run();

        converter.finish();
    }
    catch(const std::runtime_error& e){
        fprintf(stderr, "TM2IN : %s\n", e.what());
        exit(EXIT_FAILURE);
    }

/*
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
    converter->doValidation();


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
*/
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

void createAndRemoveDir(const string &version, const string &resultPath) {
    if (boost::filesystem::exists(resultPath)){
        if (boost::filesystem::exists(resultPath + version)){
            char ans;
            cout << "\n\nThis version " << version << " folder exist. Remove Files in directory? (y/n)" << endl;
            cin >> ans;
            if (ans == 'y' || ans == 'Y')
                boost::filesystem::remove_all(resultPath +version);
        }
        else{
            boost::filesystem::create_directory(resultPath + version);
        }
    }
    else{
        boost::filesystem::create_directory(resultPath);
        boost::filesystem::create_directory(resultPath+ version);
    }
}
