#ifndef MANAGER_H
#define MANAGER_H

#include <logic/check.hpp>
#include <fileio/GenerationWriter.h>
#include <fileio/export/SpaceExporter.h>
#include "fileio/import/Importer.h"
#include "util.h"

class Converter
{


protected:
    double startDegree = 1.0;
    void tagID();

    int checkSelfIntersection();

    //Triangle Mesh
    int initTriangleMesh();

    int mergeSurfaces();
    int processGenerations(Space *space, double &degree);
    int simplifyShareEdge();

    int convertTriangleMeshToSpace();
    int convertSpaceToTriangleMesh();

    int export3DS(const char *string);
    int exportSpace();

    int partitionTriangleMeshByComponent();
    int remainStructure();
    int handleOpenTriangleMesh();

public:
    Importer* di;
    SpaceExporter* de;
    vector<TriangleMesh*> mesh_list;
    vector<Space*> spaceList;
    GenerationWriter* generation_writer;
    map<string, string> paths;

    Converter(){};
    virtual ~Converter(){};

    int importMesh();

    virtual int pre_process() = 0;
    virtual int constructSpace() = 0;
    virtual int finish() = 0;

    void setImporter(Importer* p_di){ di = p_di; }
    void setGenerationWriter(GenerationWriter * pw){generation_writer = pw;}
    void setExporter(SpaceExporter* _de){ de = _de;}
    void setPaths(map<string, string> _paths);


private:

};

#endif // MANAGER_H
