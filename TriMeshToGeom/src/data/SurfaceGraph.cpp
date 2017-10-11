#include "data/SurfaceGraph.h"
#include <iostream>

bool SurfaceGraph::isNeighbor(ull id1, ull id2){
    for (ull i = 0 ; i < adjList[id1].size() ; i++){
        if (adjList[id1][i] == id2) return true;
    }
    return false;
}

void SurfaceGraph::makeAdjacentGraph(vector<Surface*>& surface_list){
    adjList.assign(surface_list.size(), vector<ull>());
    size = surface_list.size();
    for (ull i = 0 ; i < surface_list.size() - 1 ; i++){
        Surface* surface_i = surface_list[i];
        for (ull j = i ; j < surface_list.size() ; j ++){
            if (surface_i->isAdjacent(surface_list[j])){
                adjList[surface_i->sf_id].push_back(surface_list[j]->sf_id);
                adjList[surface_list[j]->sf_id].push_back(surface_i->sf_id);
            }
        }
    }
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
