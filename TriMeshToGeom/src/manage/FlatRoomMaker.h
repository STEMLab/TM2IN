#ifndef RectRoomMaker_H
#define RectRoomMaker_H

#include "manage/RoomMaker.h"


class FlatRoomMaker : public RoomMaker{
public:
    FlatRoomMaker();
    virtual ~FlatRoomMaker();

    int makeSurfaces(double degree);
    int cleaning(int, double startDegree);
    int makeSimpleSpaces(SpaceMaker* sm);

protected:


private:
};

#endif // RectRoomMaker_H
