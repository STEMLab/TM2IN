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

int OBJCollection::makeSurfaces(Checker* check){

    for (auto it = this->space_list.begin() ; it != this->space_list.end() ; it++)
    {
        cout << (*it) -> name << " is converting..." << endl;
        int ret = (*it)->makeSurfacesGreedy();
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

int OBJCollection::cleaning(Checker* ch, int max_gener){
    for (ull it = 0 ; it < this->space_list.size(); it++)
    {
        for (unsigned int i = 0 ; i < this->space_list[it]->polygon_list.size() ;i++){
            if (this->space_list[it]->polygon_list[i]->checkDuplicate(ch)){
                cout << "it has duplicate Vertex" << endl;
                return -1;
            }
        }

        this->space_list[it]->updateNormal();

        ll p_size = this->space_list[it]->polygon_list.size();
        double degree = 10.0;
        int gen = 0;

        while (true && max_gener--)
        {
            cout << "generation : " << gen << endl;
            if (this->space_list[it]->combineSurface(degree) == -1)
            {
                cout << "combine error" << endl;
                return -1;
            }
            if (degree < 45) degree += 2.5;
            this->space_list[it]->tagID();
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

            if (p_size == this->space_list[it]->polygon_list.size()) break;
            else p_size = this->space_list[it]->polygon_list.size();

            test(gen);
            gen++;

        }

        this->space_list[it]->updateNormal();

        degree = 45.0;
        max_gener = 5;
        ch->ori_degree = 20.0;
        while (true && max_gener--)
        {
            cout << "generation : " << gen << endl;
            if (this->space_list[it]->combineSurfaceByArea(degree) == -1)
            {
                cout << "combine error" << endl;
                return -1;
            }
            if (degree < 60) degree += 2.0;
            this->space_list[it]->tagID();
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
            if (p_size == this->space_list[it]->polygon_list.size()) break;
            else p_size = this->space_list[it]->polygon_list.size();

            test(gen);
            gen++;
        }
        ch->ori_degree = 10.0;

//        if (this->space_list[it]->makeCoplanar() == -1)
//        {
//            cout << "makeCoplanar error" << endl;
//            return -1;
//        }
//        if (this->space_list[it]->match00() == -1)
//        {
//            cout << "match00 error" << endl;
//            return -1;
//        }

    }
    return 0;
}


void OBJCollection::test(int gen){
    cout << "test " << endl;
    const char result_path[50] = "../Result/generation/";

    ofstream fout;
    string f_path = string(GENERATION_PATH) + "main_g_" + to_string(gen) + ".json";
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return ;
    if (JSONMaker::printJSON(fout, this->space_list))
    {
        return ;
    }
    fout.close();
}


