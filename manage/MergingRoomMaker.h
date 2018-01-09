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
    virtual int constructSpace(CombineParameter* );
    virtual int makeSimpleSpaces(SpaceMaker* sm);
    virtual int rotateSurfaces();
protected:


private:

    int process_generation(Space* space, int& maxGeneration, int& currentGeneration, double& degree);
};

#endif // RectRoomMaker_H
