#ifndef RectRoomMaker_H
#define RectRoomMaker_H

#include "manage/RoomMaker.h"


class FlatRoomMaker : public RoomMaker{
public:
    FlatRoomMaker();
    virtual ~FlatRoomMaker();

    int import(const char*);
    int makeSurfaces(double degree);
    int cleaning(int, double startDegree);
    //import and export
    int exportJSON(string f_path);
    int exportCombined(string f_path);
    int importCombined(string f_path);
    int makeSolids(double degree);

protected:


private:
};

#endif // RectRoomMaker_H
