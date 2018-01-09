//
// Created by dongmin on 18. 1. 9.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_COMBINEPARAMETER_H
#define TRIANGLEMESHTOCLEARSOLID_COMBINEPARAMETER_H


class CombineParameter{
public:
    int maxGEN;
    double startDegree;
    bool simplifyLine;
    bool snapSurface;

    CombineParameter(int _mg, int _sd, bool _sl, bool _ssf){
        maxGEN = _mg;
        startDegree = _sd;
        simplifyLine = _sl;
        snapSurface = _ssf;
    };
};

#endif //TRIANGLEMESHTOCLEARSOLID_COMBINEPARAMETER_H
