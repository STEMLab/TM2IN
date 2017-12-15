#include "manage/FlatRoomMaker.h"

#include "fileio/JSONMaker.h"
#include "fileio/CombinedIO.h"
#include <fstream>

using namespace std;

FlatRoomMaker::FlatRoomMaker()
{
    //ctor
}

FlatRoomMaker::~FlatRoomMaker()
{
    //dtor
}

int FlatRoomMaker::pre_process(double degree){
    return objcl->makeTriangleToSurface(degree);
}


int FlatRoomMaker::constructSpace(CombineParameter* cp){
    if (objcl->combineSurfaces(check, cp) == -1) return -1;
    //objcl->rotateSurfaces();
    //if (objcl->finish()) return -1;
    return 0;
}

int FlatRoomMaker::makeSimpleSpaces(SpaceMaker* sm){
    if (objcl -> makeSimpleSpaces(sm) ) return -1;
    return 0;
}

