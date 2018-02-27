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
    virtual int rotateSurfaces();
protected:


private:
    const double startDegree = 1.0;
    const int maxGeneration = 20;
    const bool simplifyLine = false;
    const bool snapMode = false;

    int checkClosedSurface();

    int processGenerations(Space *space, int &currentGeneration, double &degree);
    int mergeSurfaces();

    int fillHoleWithUsingPolyhedralSurface();

    int makeSurfacesPlanar();

    int resolveIntersection();

    int updateVertexList();

    int triangulation();

    int checkSelfIntersection();
};

#endif // RectRoomMaker_H
