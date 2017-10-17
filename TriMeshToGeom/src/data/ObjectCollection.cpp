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

int OBJCollection::combineSurfaces(Checker* ch, int max_gener, double startDegree){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        for (unsigned int i = 0 ; i < this->space_list[it]->surfacesList.size() ;i++){
            if (this->space_list[it]->surfacesList[i]->checkDuplicate(ch)){
                cout << "it has duplicate Vertex" << endl;
                return -1;
            }
        }

        ll p_size = this->space_list[it]->surfacesList.size();
        this->process_writer->writeRoughSurfaces(p_size, it);

        double degree = startDegree;
        int gen = 0;

        while (true && max_gener--){
            this->space_list[it]->updateNormal();
            cout << "generation : " << gen << endl;
            if (this->space_list[it]->combineSurface(degree) == -1)
            {
                cout << "combine error" << endl;
                return -1;
            }
            if (degree < 45) degree += 0.05;
            if (this->space_list[it]->simplifySegment() == -1)
            {
                cout << "simplify error" << endl;
                return -1;
            }
            if (this->space_list[it]->handleDefect() == -1)
            {
                cout << "" << endl;
                return -1;
            }

            if (p_size == (int)this->space_list[it]->surfacesList.size()) break;
            else p_size = (int)this->space_list[it]->surfacesList.size();

            this->process_writer->writeGenerationJSON(gen, space_list);
            this->process_writer->writeGeneration(gen, this->space_list[it]->surfacesList.size(), it);
            gen++;

        }

        //this->space_list[it]->updateNormal();

        if (this->space_list[it]->match00() == -1)
        {
            cout << "match00 error" << endl;
            return -1;
        }

       // this->space_list[it]->remainOnlyUsingVertices();

    }
    return 0;
}

int OBJCollection::makeSurfacesPlanar(){
    for (ull i = 0 ; i < this->space_list.size(); i++)
    {
        this->space_list[i]->makeSurfacesPlanar();
    }
    return 0;
}

int OBJCollection::makeGraph(){
    for (ull i = 0 ; i < this->space_list.size(); i++)
    {
        this->space_list[i]->tagID();
        this->space_list[i]->makeGraph();
    }
    return 0;
}

int OBJCollection::makeWall(double degree){
    for (ull i = 0 ; i < this->space_list.size(); i++)
    {
        this->space_list[i]->makeWallRectangle();
        this->space_list[i]->makeClosedWall();
    }
    return 0;
}

int OBJCollection::makeFloorAndCeiling(){
    for (ull i = 0 ; i < this->space_list.size() ; i++){
        this->space_list[i]->makeFloorAndCeiling();
    }
    return 0;
}

int OBJCollection::makeSolid(){
    cout << "make Solid" << endl;
    free();
    for (ull i = 0 ; i < this->space_list.size();i++){
        this->space_list[i]->getVertexList(this->vertex);
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

void OBJCollection::test(){
    Surface* zero = this->space_list[0]->surfacesList[0];
    for (int i = 0 ; i < (int)this->space_list[0]->surfacesList.size() ; i++){
        Surface* sf = this->space_list[0]->surfacesList[i];
        if (sf->sf_id == 36)
        {
            double angle = CGALCalculation::getAngle(zero->av_normal, sf->av_normal);
            Vector_3 added = zero->av_normal + sf->av_normal;
            cout << angle << endl;
            cout << (CGALCalculation::getAngle(added, zero->av_normal)) << endl;
        }
    }
}

