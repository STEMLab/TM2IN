#include "features/SurfaceGraph.h"
#include <iostream>

bool SurfaceGraph::isNeighbor(ull id1, ull id2){
    for (ull i = 0 ; i < adjList[id1].size() ; i++){
        if (adjList[id1][i] == id2) return true;
    }
    return false;
}

void SurfaceGraph::attachNewTriagle(vector<Triangle>& tri_list){
    vector<ull> open_list;
    for (ull i = 0; i < adjList.size() ; i++){
        if (adjList[i].size() > 3){
            cout << adjList[i].size() <<endl;
            assert(false);
        }
        if (adjList[i].size() < 3){
            open_list.push_back(i);
        }
    }

}

void SurfaceGraph::makeAdjacentGraph(vector<Surface*>& surface_list){
    adjList.assign(surface_list.size(), vector<ull>());
    for (ull i = 0 ; i < surface_list.size() - 1 ; i++){
        Surface* surface_i = surface_list[i];
        for (ull j = i + 1 ; j < surface_list.size() ; j ++){
            if (surface_i->isAdjacent(surface_list[j])){
                adjList[surface_i->sf_id].push_back(surface_list[j]->sf_id);
                adjList[surface_list[j]->sf_id].push_back(surface_i->sf_id);
            }
        }
    }
}

void SurfaceGraph::makeAdjacentGraph(vector<Triangle>& tri_list){
    adjList.assign(tri_list.size(), vector<ull>());
    for (ull i = 0 ; i < tri_list.size() - 1 ; i++){
        printProcess(i, tri_list.size(), "makeGraph");
        for (ull j = i + 1 ; j < tri_list.size() ; j ++){
            if (tri_list[i].isAdjacent(tri_list[j])){
                adjList[i].push_back(j);
                adjList[j].push_back(i);
            }
        }
    }
}

bool SurfaceGraph::isClosedTriangleMesh(){
    for (ull i = 0; i < adjList.size() ; i++){
        if (adjList[i].size() != 3){
            return false;
        }
    }
    return true;
}

void SurfaceGraph::print_bfs(){
    vector<bool> checked(this->size, false);

    for (unsigned int i = 0 ; i < this->size ; i ++){
        if (checked[i]) continue;
        queue<int> wait_queue;
        wait_queue.push(i);
        checked[i] = true;

        int level = 0;
        cout << i << endl;

        while (wait_queue.size() > 0){
            int current = wait_queue.front();
            wait_queue.pop();

            level += 1;

            for (unsigned int nb = 0 ; nb < adjList[current].size() ; nb++){
                ull next_surface = adjList[current][nb];
                if (checked[next_surface]) continue;
                else{
                    checked[next_surface] = true;
                    cout << next_surface << " ";
                    wait_queue.push(next_surface);
                }
            }
        }

        cout << "------------\n" << endl;
    }

}
