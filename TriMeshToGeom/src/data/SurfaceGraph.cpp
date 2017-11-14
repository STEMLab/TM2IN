#include "data/SurfaceGraph.h"
#include <iostream>

void SurfaceGraph::makeAdjacentGraph(vector<Surface*>& surface_list){
    adjList.assign(surface_list.size(), vector<pair<ull, ii>>());
    size = surface_list.size();
    for (ull i = 0 ; i < surface_list.size() - 1 ; i++){
        Surface* surface_i = surface_list[i];
        for (ull j = i ; j < surface_list.size() ; j ++){
            ll s_i, s_j;
            if (surface_i->isAdjacent(surface_list[j], s_i, s_j)){
                adjList[i].push_back(make_pair(j, make_pair(s_i,s_j)));
                adjList[j].push_back(make_pair(i, make_pair(s_j,s_i)));
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

            for (int nb = 0 ; nb < adjList[current].size() ; nb++){
                int next_surface = adjList[current][nb].first;
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
