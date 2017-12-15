//
//  ObjectCollection.h
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef ObjectCollection_h
#define ObjectCollection_h

#include "model/Surface.hpp"
#include "logic/check.hpp"
#include "space_maker/SpaceMaker.h"
#include "data/Space.h"
#include "fileio/JSONMaker.h"
#include "predefine.h"
#include "fileio/GenerationWriter.h"

#include <vector>
#include <cstring>
#include <string>

using namespace std;
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

class OBJCollection{
public:
    vector<Vertex*> vertex;
    vector<Space*> space_list;
    vector<Space*> simple_space_list;
    GenerationWriter* generation_writer;

    void setWriter(GenerationWriter * pw){generation_writer = pw;}

    int makeTriangleToSurface(double degree);

    int combineSurfaces(Checker* ch, CombineParameter* cp);
    int rotateSurfaces();
    int finish();

    int makeSimpleSpaces(SpaceMaker* sm);
    int clusterAndMakeSurfaces(double degree);

    void free();
private:
    int process_generation(Space* space, int& maxGeneration, int& currentGeneration, double& degree);
};

#endif /* ObjectCollection_h */
