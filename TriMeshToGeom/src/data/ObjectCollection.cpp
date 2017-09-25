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

int OBJCollection::makeSurfaces(Checker* check, double degree){

    for (auto it = this->space_list.begin() ; it != this->space_list.end() ; it++)
    {
        cout << (*it) -> name << " is converting..." << endl;
        int ret = (*it)->makeSurfacesGreedy(degree);
        if (ret)
        {
            cout << "make Surfaces error" << endl;
            return -1;
        }

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

int OBJCollection::makeGraph(Checker* ch){
    for (ull i = 0 ; i < this->space_list.size(); i++)
    {
        this->space_list[i]->makeGraph(ch);
    }
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
        double degree = startDegree;
        int gen = 0;

        while (true && max_gener--)
        {
            this->space_list[it]->updateNormal();
            cout << "generation : " << gen << endl;

            if (max_gener == 0){
                ch->degreeOfMerging = 90.0;
                degree = 0.0001;
            }

            if (this->space_list[it]->combineSurface(degree) == -1)
            {
                cout << "combine error" << endl;
                return -1;
            }
            if (degree < 45) degree += 0.05;
            //this->space_list[it]->tagID();

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

            extractGeneration(gen);
            gen++;

        }

        this->space_list[it]->updateNormal();

//        if (this->space_list[it]->makeCoplanar() == -1)
//        {
//            cout << "makeCoplanar error" << endl;
//            return -1;
//        }
        if (this->space_list[it]->match00() == -1)
        {
            cout << "match00 error" << endl;
            return -1;
        }

    }
    return 0;
}

void OBJCollection::extractGeneration(int gen){
    ofstream fout;
    string f_path = string(GENERATION_PATH) + "office_g_" + to_string(gen) + ".json";
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return ;
    if (JSONMaker::printJSON(fout, this->space_list))
    {
        return ;
    }
    fout.close();
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

