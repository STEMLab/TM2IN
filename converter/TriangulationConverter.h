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
protected:


private:
    int triangulation();

    void makeSurfaceGraph();


};

#endif // RectRoomMaker_H
