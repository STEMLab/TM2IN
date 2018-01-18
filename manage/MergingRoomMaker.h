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

    virtual int pre_process(double degree);
    virtual int constructSpace();
    virtual int finish();
    virtual int rotateSurfaces();
protected:


private:
    const double startDegree = 1.0;
    const int maxGeneration = 20;
    const bool simplifyLine = false;
    const bool snapMode = false;

    int process_generation(Space* space, int& maxGeneration, int& currentGeneration, double& degree);
};

#endif // RectRoomMaker_H
