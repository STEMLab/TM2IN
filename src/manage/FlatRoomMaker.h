#ifndef RectRoomMaker_H
#define RectRoomMaker_H

#include "manage/RoomMaker.h"


class FlatRoomMaker : public RoomMaker{
public:
    FlatRoomMaker();
    virtual ~FlatRoomMaker();

    int pre_process(double degree);
    int constructSpace(int, double startDegree);
    int makeSimpleSpaces(SpaceMaker* sm);

protected:


private:
};

#endif // RectRoomMaker_H
