#ifndef MANAGER_H
#define MANAGER_H

#include "Options.h"
#include "util.h"
#include "algorithm/Polygonizer.h"
#include "features/IndoorComponent.h"

using namespace TM2IN;

class Converter
{
protected:
public:
    vector<Room*> rooms;

    Converter();
    virtual ~Converter(){};

    /**
     * Ready to run projects. Triangle Mesh data -> TM2IN::TriangleMesh Class Objects
     */
    int start();

    /**
     * Converts data. TM2IN::TriangleMesh -> TM2IN::TriangulatedSurfaceMesh
     */
    int run();

    /**
     * Finish and export data. TM2IN::TriangulatedSurfaceMesh -> TM2IN::PolygonMesh
     * @return Succeed : 0
     */
    int finish();
private:
    // start
    int importData();
    void print_input_spec();

    // run
    int mergeSurfaces();
    void make_tri_surface_mesh(Room*);
    int validate_tsm();
    int polygonize();
    TM2IN::algorithm::Polygonizer *create_polygonizer();
    int classify_sf_type();

    // finish
    void tag_pm_ID();
    int convert_pm_to_tm();
    int exportRoomBoundary();

    void validate_tm();
};

#endif // MANAGER_H
