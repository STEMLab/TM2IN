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

    int constructMeshGraph();

    int processGenerations(Space *space, double &degree);
    int mergeSurfaces();

    int triangulation();

    int checkSelfIntersection();

    int remainStructure();

    bool resolveWrongTriangle();

    int simplifyShareEdge();

    int groupByClosedMesh();

    int export3DS(const char *string);
};

#endif // RectRoomMaker_H
