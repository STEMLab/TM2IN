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
     * starting to convert data. Triangle Mesh data -> TM2IN::TriangleMesh Class Objects
     */
    int start();

    /**
     * converting data. TM2IN::TriangleMesh -> TM2IN::TriangulatedSurfaceMesh
     * @return
     */
    int run();

    /**
     * finish and export data. TM2IN::TriangulatedSurfaceMesh -> TM2IN::PolygonMesh
     * @return
     */
    int finish();
private:
    // start
    int importData();
    void print_input_spec();
    int handleOpenTriangleMesh();

    // run
    int mergeSurfaces();
    RoomBoundary::TriangulatedSurfaceMesh *make_tri_surface_mesh(RoomBoundary::TriangleMesh *tm);
    int validate_tsm();
    int polygonize();
    TM2IN::algorithm::Polygonizer *create_polygonizer();

    // finish
    void tag_pm_ID();
    int convert_pm_to_tm();
    int exportRoomBoundary();

};

#endif // MANAGER_H
