#ifndef RectRoomMaker_H
#define RectRoomMaker_H

#include "Converter.h"


class TriangulationConverter : public Converter{
public:
    TriangulationConverter(){}
    ~TriangulationConverter(){}

    virtual int pre_process();
    virtual int constructSpace();
    virtual int finish();

    int triangulation();

protected:


private:

};

#endif // RectRoomMaker_H
