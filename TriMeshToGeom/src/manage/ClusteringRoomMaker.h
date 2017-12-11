#ifndef CLUSTERINGROOMMAKER_H_INCLUDED
#define CLUSTERINGROOMMAKER_H_INCLUDED

#include "RoomMaker.h"

class ClusteringRoomMaker : public RoomMaker{
public:
    int makeSurfaces(double degree);
    int cleaning(int, double startDegree);
    int makeSimpleSpaces(SpaceMaker* sm);

};

#endif // CLUSTERINGROOMMAKER_H_INCLUDED
