#ifndef MANAGER_H
#define MANAGER_H

#include <logic/check.h>
#include <fileio/GenerationWriter.h>
#include <fileio/export/DataExporter.h>
#include "fileio/MeshImporter.h"
#include "util.h"
#include "polygonizer/Polygonizer.h"

class Converter
{
protected:
public:
    vector<TriangleMesh*> mesh_list;
    vector<PolyhedralSurface*> spaceList;
    GenerationWriter* generation_writer;
    map<string, string> paths;

    Converter(){};
    virtual ~Converter(){};

    int import3DS(const char *file_path) {
        this->mesh_list = MeshImporter::import3DS(file_path);
        if (this->mesh_list.size() == 0) return -1;
        return 0;
    }

    int importDAE(string file_path) {
        this->mesh_list = MeshImporter::importDAE(file_path);
        if (this->mesh_list.size() == 0) return -1;
    }

    int importTVR(string file_path) {
        this->mesh_list = MeshImporter::importTVR(file_path);
        if (this->mesh_list.size() == 0) return -1;
    }

    int finish();

    void tagID();

    int checkSelfIntersection();

    //Triangle Mesh
    int initTriangleMesh();

    int mergeSurfaces();
    int processGenerations(PolyhedralSurface *space);
    int simplifyShareEdge();

    int convertTriangleMeshToSpace();
    int convertSpaceToTriangleMesh();

    int exportSpace();

    int partitionTriangleMeshByComponent();
    int remainSelectedMesh(int arch);
    int handleOpenTriangleMesh();

    void makeSurfaceGraph();

    void setImporter(MeshImporter* p_di){ di = p_di; }
    void setGenerationWriter(GenerationWriter * pw){generation_writer = pw;}
    void setExporter(DataExporter* _de){ de = _de;}
    void setPaths(map<string, string> _paths);

    int export3DS();

    int polygonize(Polygonizer *polygonizer);
    int triangulation();

    void printInputDataSpec();
};

#endif // MANAGER_H
