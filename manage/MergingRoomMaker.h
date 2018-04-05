#ifndef RectRoomMaker_H
#define RectRoomMaker_H

#include "manage/RoomMaker.h"


class MergingRoomMaker : public RoomMaker{
public:

    MergingRoomMaker(){
        cout << "MergingRoomMaker" << endl;
    }
    ~MergingRoomMaker(){

    }

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

};

#endif // RectRoomMaker_H
