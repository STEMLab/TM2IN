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

int OBJCollection::makeTriangleToSurface(double degree){
    for (ull s_i = 0 ; s_i < this->space_list.size(); s_i++)
    {

        for (ull j = 0 ; j < this->space_list[s_i]->triangles.size() ; ){
            if (this->space_list[s_i]->triangles[j].a == this->space_list[s_i]->triangles[j].b ||
                this->space_list[s_i]->triangles[j].a == this->space_list[s_i]->triangles[j].c ||
                this->space_list[s_i]->triangles[j].c == this->space_list[s_i]->triangles[j].b){
                cout << "Wrong Triangle" << endl;
                return -1;
            }
            else{
                j++;
            }
        }
/*
        cout << "make Triangle Graph" << endl;
        SurfaceGraph* sg = new SurfaceGraph();
        sg->makeAdjacentGraph(this->space_list[s_i]->triangles);
        for (ull i = 0 ; i < sg->adjList.size() ; i++){
            if (sg->adjList[i].size() != 3){
                cout << i << " : " << sg->adjList[i].size() << endl;
            }
        }
        if (!sg->isClosedTriangleMesh()){
            cout << "Not Closed Polyhedral" << endl;
            sg->attachNewTriagle(this->space_list[s_i]->triangles);
            if (!sg->isClosedTriangleMesh()) return -1;
        }

*/
        cout << this->space_list[s_i]->triangles.size() << endl;

        cout << this->space_list[s_i] -> name << " is converting..." << endl;
        //if (this->space_list[i]->mergeTrianglesGreedy(degree)){
        if (this->space_list[s_i]->changeTrianglesToSurfaces()){
            cout << "make Surfaces error" << endl;
            return -1;
        }
    }

    return 0;
}

int OBJCollection::process_generation(Space* space, int& maxGeneration, int& currentGeneration, double& degree){
    ll p_size = space->surfacesList.size();
    while (true && maxGeneration-- > 0){
        cout << "generation : " << currentGeneration << endl;
        if (space->combineSurface(degree) == -1)
        {
            cout << "combine error" << endl;
            return -1;
        }

//        if (space->snapSurface(0.01) == -1){ cout << "snap Surface" << endl; return -1;}
//        if (space->handleDefect(0.1) == -1){ cout << "cannot handle defect" << endl; return -1; }

        if (p_size == (int)space->surfacesList.size()) {
            cout << "generation " << currentGeneration  << " done.. "<< endl;
            break;
        }
        else p_size = (int)space->surfacesList.size();

        currentGeneration++;
        this->generation_writer->writeGenerationJSON(currentGeneration, this->space_list);
        this->generation_writer->writeGenerationStat(currentGeneration, space->surfacesList.size());
        if (degree < 15) degree += 0.05;
    }
    return 0;
}

int OBJCollection::combineSurfaces(Checker* ch, CombineParameter* cp){
    int maxGENperOneCycle = cp->maxGEN;
    double startDegree = cp->startDegree;
    bool simplify_mode = cp->simplifyLine;
    bool snap_mode = cp->snapSurface;

    double diff = 0.0001;

    if (this->space_list.size() == 0) cout << "there is no space" << endl;
    int temp_maxGENperOneCycle = maxGENperOneCycle;
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        this->generation_writer->clearGenerationStat();
        Space* space = this->space_list[it];
        for (unsigned int s_i = 0 ; s_i < this->space_list[it]->surfacesList.size() ;s_i++){
            if (space->surfacesList[s_i]->checkDuplicate(ch)){
                cout << "it has duplicate Vertex" << endl;
                return -1;
            }
        }

        double degree = startDegree;
        double angle = 0.1;
        int gen = 0;
        this->generation_writer->writeGenerationJSON(gen, space_list);
        this->generation_writer->writeGenerationStat(gen, space->surfacesList.size());

        while (true){
            ll p_size = space->surfacesList.size();
            if (process_generation(space, maxGENperOneCycle, gen, degree)) return -1;
            if (p_size == (int)space->surfacesList.size()) {
                cout << "generation " << gen  << " done.. "<< endl;
                break;
            }
            maxGENperOneCycle = temp_maxGENperOneCycle;

            if (snap_mode){
                if (space->snapSurface(diff) == -1){ cout << "snap Surface" << endl; return -1;}
                if (space->handleDefect(angle) == -1){ cout << "cannot handle defect" << endl; return -1; }
            }

            //angle = 1.0;
            cout << "simplify and handleDefect" << endl;
            if (simplify_mode)
                if (space->simplifySegment() == -1){ cout << "simplify error" << endl; return -1;}
            if (space->handleDefect(angle) == -1){ cout << "cannot handle defect" << endl; return -1; }

        }

        sort(space->surfacesList.begin(), space->surfacesList.end(), Surface::compareArea);
        SLC::tagID(space->surfacesList);

        Vector_3 vc1(space->surfacesList[0]->v_list[577]->getCGALPoint(), space->surfacesList[0]->v_list[578]->getCGALPoint());
        Vector_3 vc2(space->surfacesList[0]->v_list[577]->getCGALPoint(), space->surfacesList[0]->v_list[579]->getCGALPoint());
        cout << CGALCalculation::getAngle(vc1, vc2) << endl;
    }
    return 0;
}



int OBJCollection::rotateSurfaces(){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        Space* space = this->space_list[it];
        space->rotateSpaceByFloorTo00();
        if (space->match00() == -1){
            cout << "match00 error" << endl;
            return -1;
        }
    }
    return 0;
}


int OBJCollection::finish(){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        Space* space = this->space_list[it];
        space->updateNormal();

    }
}

int OBJCollection::makeSimpleSpaces(SpaceMaker* sm){
    for (ull s_i = 0 ; s_i < this->space_list.size();s_i++){
        Space* space = this->space_list[s_i];
        space->updateNormal();
        //Space* new_space = new Space(space->name, space->checker);
        sm->checker = space->checker;
        space->surfacesList = sm->makeSimpleSurfaces(space->surfacesList);
        //this->simple_space_list.push_back(new_space);
    }

    return 0;
}

void OBJCollection::free(){
    sort( vertex.begin(), vertex.end() );
    vertex.erase( unique( vertex.begin(), vertex.end() ), vertex.end() );

    for (int s_i = 0 ; s_i < (int)this->vertex.size() ; s_i++){
        if (this->vertex[s_i] != NULL){
            delete this->vertex[s_i];
            this->vertex[s_i] = NULL;
        }

    }
    vertex.clear();
}

int OBJCollection::clusterAndMakeSurfaces(double degree){
    for (ull s_i = 0 ; s_i < this->space_list.size(); s_i++)
    {
        this->space_list[s_i]->surfacesList = TriangleCalculation::clusterAndmakeSurfaces(this->space_list[s_i]->triangles);
        this->space_list[s_i]->match00();

    }

    return 0;

}
