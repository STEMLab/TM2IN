#ifndef MANAGER_H
#define MANAGER_H

#include <logic/check.hpp>
#include <fileio/GenerationWriter.h>
#include <fileio/export/SpaceExporter.h>
#include "fileio/import/Importer.h"
#include "util.h"

class RoomMaker
{
public:
    Importer* di;
    SpaceExporter* de;
    vector<TriangleMesh*> mesh_list;
    // vector<Vertex*> vertices;
    vector<Space*> spaceList;
    GenerationWriter* generation_writer;
    map<string, string> paths;

    RoomMaker(){};
    virtual ~RoomMaker(){};

    int importMesh();
    int convertTriangleMeshToSpace();
    int convertSpaceToTriangleMesh();

    virtual int pre_process() = 0;
    virtual int constructSpace() = 0;
    virtual int finish() = 0;

    void setImporter(Importer* p_di){ di = p_di; }
    void setGenerationWriter(GenerationWriter * pw){generation_writer = pw;}

    int setExporter(SpaceExporter* _de){ de = _de;}
    int exportSpace();

    void setPaths(map<string, string> _paths);

    int export3DS(const char *string);
};

#endif // MANAGER_H
