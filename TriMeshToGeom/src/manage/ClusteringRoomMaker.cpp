#include "ClusteringRoomMaker.h"


int ClusteringRoomMaker::pre_process(double degree){
    return this->objcl->clusterAndMakeSurfaces(degree);
}


int ClusteringRoomMaker::constructSpace(int max_gener, double startDegree){
    return 0;
}

int ClusteringRoomMaker::makeSimpleSpaces(SpaceMaker* sm){
    return 0;
}

