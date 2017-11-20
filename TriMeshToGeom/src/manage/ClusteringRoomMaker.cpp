#include "ClusteringRoomMaker.h"


int ClusteringRoomMaker::makeSurfaces(double degree){
    return this->objcl->clusterAndMakeSurfaces(degree);
}


int ClusteringRoomMaker::cleaning(int max_gener, double startDegree){
    return 0;
}

int ClusteringRoomMaker::makeSimpleSpaces(SpaceMaker* sm){
    return 0;
}

