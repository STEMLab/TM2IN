#ifndef MANAGER_H
#define MANAGER_H

#include <logic/check.h>
#include <fileio/GenerationWriter.h>
#include <fileio/export/SpaceExporter.h>
#include "fileio/import/Importer.h"
#include "util.h"
#include "polygonizer/Polygonizer.h"

class Converter
{
protected:


public:
    Importer* di;
    SpaceExporter* de;
    vector<TriangleMesh*> mesh_list;
    vector<Solid*> spaceList;
    GenerationWriter* generation_writer;
    map<string, string> paths;

    Converter(){};
    virtual ~Converter(){};

    int importMesh();

    int finish();

    void tagID();

    int checkSelfIntersection();

    //Triangle Mesh
    int initTriangleMesh();

    int mergeSurfaces();
    int processGenerations(Solid *space);
    int simplifyShareEdge();

    int convertTriangleMeshToSpace();
    int convertSpaceToTriangleMesh();

    int exportSpace();

    int partitionTriangleMeshByComponent();
    int remainSelectedMesh(int arch);
    int handleOpenTriangleMesh();

    void makeSurfaceGraph();

    void setImporter(Importer* p_di){ di = p_di; }
    void setGenerationWriter(GenerationWriter * pw){generation_writer = pw;}
    void setExporter(SpaceExporter* _de){ de = _de;}
    void setPaths(map<string, string> _paths);

    int export3DS();

    int polygonize(Polygonizer *polygonizer);
    int triangulation();

    void printInputDataSpec();
};

#endif // MANAGER_H
