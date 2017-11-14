//
//  TVRCollection.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "data/ObjectCollection.h"


#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define FLOATSIZE 1000000

using namespace std;

int OBJCollection::makeSurfaces(double degree){

    for (ull i = 0 ; i < this->space_list.size(); i++)
    {
        this->process_writer->writeBeforeJoin(this->space_list[i]->triangles.size(), i);
        cout << this->space_list[i] -> name << " is converting..." << endl;
        int ret = this->space_list[i]->makeSurfacesGreedy(degree);
        if (ret)
        {
            cout << "make Surfaces error" << endl;
            return -1;
        }

    }

    return 0;
}

int OBJCollection::combine_simplify_handle(Space* space, double degree){
    if (space->combineSurface(degree) == -1)
    {
        cout << "combine error" << endl;
        return -1;
    }
    if (space->simplifySegment() == -1)
    {
        cout << "simplify error" << endl;
        return -1;
    }
    if (space->handleDefect() == -1)
    {
        cout << "" << endl;
        return -1;
    }
    return 0;
}

int OBJCollection::combineSurfaces(Checker* ch, int max_gener, double startDegree){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        Space* space = this->space_list[it];
        for (unsigned int i = 0 ; i < this->space_list[it]->surfacesList.size() ;i++){
            if (space->surfacesList[i]->checkDuplicate(ch)){
                cout << "it has duplicate Vertex" << endl;
                return -1;
            }
        }

        ll p_size = space->surfacesList.size();
        //this->process_writer->writeRoughSurfaces(p_size, it);

        double degree = startDegree;
        int gen = 0;

        while (true && max_gener--){
            space->updateNormal();
            cout << "generation : " << gen << endl;

            if (this->combine_simplify_handle(space, degree)) return -1;

            if (p_size == (int)space->surfacesList.size()) break;
            else p_size = (int)space->surfacesList.size();

            this->process_writer->writeGenerationJSON(gen, space_list);

            gen++;
            if (degree < 15) degree += 0.05;
        }

        if (this->space_list[it]->match00() == -1)
        {
            cout << "match00 error" << endl;
            return -1;
        }

        this->space_list[it]->tagID();
        this->space_list[it]->makeGraph();
    }
    return 0;
}

int OBJCollection::makeSimpleSpaceGreedy(){
    for (ull i = 0 ; i < this->space_list.size(); i++)
    {
        Space* space = this->space_list[i];
        space->updateNormal();
        space->removeSurfacesNotConnectedFC();
        space->removeOppositeSurfaces();
        space->makeSurfacesPlanarWithLowest();
        space->makeWallRectangle();
        space->makeClosedWall();
        space->makeFloorAndCeiling();
    }
    return 0;
}


int OBJCollection::makeSolid(double degree){
    cout << "make Solid" << endl;
    for (ull i = 0 ; i < this->space_list.size();i++){
        Space* space = this->space_list[i];
        space->solid = new Solid();
        space->solid->surfacesList = space->getTopSurfaces(degree);
    }

    return 0;
}

void OBJCollection::free(){
    sort( vertex.begin(), vertex.end() );
    vertex.erase( unique( vertex.begin(), vertex.end() ), vertex.end() );

    for (int i = 0 ; i < (int)this->vertex.size() ; i++){
        if (this->vertex[i] != NULL){
            delete this->vertex[i];
            this->vertex[i] = NULL;
        }

    }
    vertex.clear();
}


int OBJCollection::rotateSurfaces(){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        Space* space = this->space_list[it];
        space->rotateSpaceByFloorTo00();
    }
    return 0;
}
