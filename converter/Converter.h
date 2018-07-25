#ifndef MANAGER_H
#define MANAGER_H

#include "Options.h"
#include "util.h"
#include "algorithm/Polygonizer.h"
#include "features/Geometry.h"

using namespace TM2IN;

class Converter
{
protected:
    Options& options;
public:
    vector<TM2IN::TriangleMesh*> mesh_list;
    vector<TM2IN::PolyhedralSurface*> spaceList;

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
    int processGenerations(TM2IN::PolyhedralSurface *space);

    int convertTriangleMeshToSpace();
    int convertSpaceToTriangleMesh();

    int exportSpace();

    int partitionTriangleMeshByComponent();
    int remainSelectedMesh(int arch);
    int handleOpenTriangleMesh();

    void printInputDataSpec();

    int polygonize();

    TM2IN::algorithm::Polygonizer *create_polygonizer();
};

#endif // MANAGER_H
