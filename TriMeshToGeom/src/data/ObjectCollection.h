//
//  ObjectCollection.h
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef ObjectCollection_h
#define ObjectCollection_h

#include "data/Surface.hpp"
#include "logic/check.hpp"
#include "data/Space.h"
#include "fileio/JSONMaker.h"
#include "predefine.h"
#include "fileio/ProcessWriter.h"

#include <vector>
#include <cstring>
#include <string>

using namespace std;

class OBJCollection{
public:
    vector<Vertex*> vertex;
    vector<Space*> space_list;
    ProcessWriter* process_writer;

    void setWriter(ProcessWriter * pw){process_writer = pw;}

    int makeSurfaces(double degree);
    void free();
    int combineSurfaces(Checker* ch, int max_gener, double startDegree);
    int rotateSurfaces();

    int makeSimpleSpaceGreedy();
    int makeSolid();
private:
    int combine_simplify_handle(Space* space, double degree);
};

#endif /* ObjectCollection_h */
