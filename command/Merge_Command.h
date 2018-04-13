#ifndef RectRoomMaker_H
#define RectRoomMaker_H

#include "Command.h"


class Merge_Command : public Command{
public:
    Merge_Command(){}
    ~Merge_Command(){}

    virtual int pre_process();
    virtual int constructSpace();
    virtual int finish();
protected:


private:
    double startDegree = 1.0;
    int processGenerations(Space *space, double &degree);
    int mergeSurfaces();
    int triangulation();
    int checkSelfIntersection();

    //Triangle Mesh
    int menifestTriangleMesh();
    int remainStructure();
    int partitionTriangleMeshByComponent();

    int simplifyShareEdge();

    void makeSurfaceGraph();

    void tagID();

    int handleOpenTriangleMesh();
};

#endif // RectRoomMaker_H
