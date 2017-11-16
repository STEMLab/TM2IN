//
//  TVRCollection.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 21..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include "data/ObjectCollection.h"

#include "space_maker/SpaceMaker.h"
#include "logic/SurfacesListCalculation.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define FLOATSIZE 1000000

using namespace std;

int OBJCollection::mergeTriangles(double degree){

    for (ull i = 0 ; i < this->space_list.size(); i++)
    {
//        cout << "make Triangle Graph" << endl;
//        SurfaceGraph* sg = new SurfaceGraph();
//        sg->makeAdjacentGraph(this->space_list[i]->triangles);
//        if (!sg->isClosedTrinagleMesh()){
//            cout << "Not Closed Polyhedral" << endl;
//            return -1;
//        }
        cout << this->space_list[i] -> name << " is converting..." << endl;
        if (this->space_list[i]->mergeTrianglesGreedy(degree))
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
        double degree = startDegree;

        this->process_writer->writeGenerationJSON(0, space_list);

        int gen = 1;

        while (true && max_gener-- > 0){

            space->updateNormal();
            cout << "generation : " << gen << endl;

          //  if (this->combine_simplify_handle(space, degree)) return -1;
            if (space->combineSurface(degree) == -1)
            {
                cout << "combine error" << endl;
                return -1;
            }

            if (p_size == (int)space->surfacesList.size()) break;
            else p_size = (int)space->surfacesList.size();

            this->process_writer->writeGenerationJSON(gen, space_list);

            gen++;
            if (degree < 15) degree += 0.05;
        }

//        if (space->simplifySegment() == -1){
//            cout << "simplify error" << endl;
//            return -1;
//        }
        if (space->combineSurfaceMoreGreedy()) return -1;
        if (space->handleDefect() == -1){
            cout << "" << endl;
            return -1;
        }
        if (space->match00() == -1){
            cout << "match00 error" << endl;
            return -1;
        }

        SLC::tagID(space->surfacesList);
//
//        cout << space->surfacesList.size() << endl;
//
//        while (true && max_gener-- > 0){
//            this->combine_simplify_handle(space, degree);
//        }
//        cout << space->surfacesList.size() << endl;
    }
    return 0;
}

int OBJCollection::makeSimpleSpaces(SpaceMaker* sm){
    for (ull i = 0 ; i < this->space_list.size();i++){
        Space* space = this->space_list[i];
        space->updateNormal();
        Space* new_space = new Space(space->name, space->checker);
        sm->checker = space->checker;
        new_space->surfacesList = sm->makeSimpleSurfaces(space->surfacesList);
        this->simple_space_list.push_back(new_space);
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
