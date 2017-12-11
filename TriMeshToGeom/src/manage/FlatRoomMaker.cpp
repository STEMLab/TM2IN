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

int FlatRoomMaker::makeSurfaces(double degree){
    return objcl->mergeTriangles(degree);
}


int FlatRoomMaker::cleaning(int max_gener, double startDegree){
    if (objcl->combineSurfaces(check, max_gener, startDegree) == -1) return -1;
    //objcl->rotateSurfaces();
    //if (objcl->finish()) return -1;
    return 0;
}

int FlatRoomMaker::makeSimpleSpaces(SpaceMaker* sm){
    if (objcl -> makeSimpleSpaces(sm) ) return -1;
    return 0;
}

