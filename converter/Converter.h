#ifndef MANAGER_H
#define MANAGER_H

#include "logic/check.h"
#include "io/GenerationWriter.h"
#include "Options.h"
#include "util.h"
#include "algorithm/Polygonizer.h"
#include "features/Geometry.h"

class Converter
{
protected:
    Options& options;
public:
    vector<TriangleMesh*> mesh_list;
    vector<PolyhedralSurface*> spaceList;
    GenerationWriter* generation_writer;

    Converter(Options& op);
    virtual ~Converter(){};

    int start();
    int run();
    int finish();

private:
    int importMesh();

    void tagID();

    int validate();

    //Triangle Mesh
    int initTriangleMesh();

    int mergeSurfaces();
    int processGenerations(PolyhedralSurface *space);

    int convertTriangleMeshToSpace();
    int convertSpaceToTriangleMesh();

    int exportSpace();

    int partitionTriangleMeshByComponent();
    int remainSelectedMesh(int arch);
    int handleOpenTriangleMesh();

    int export3DS();


    void printInputDataSpec();

    int triangulation();
    int polygonize();

    TM2IN::algorithm::Polygonizer *create_polygonizer();
};

#endif // MANAGER_H
