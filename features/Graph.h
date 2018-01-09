#ifndef Graph_H_INCLUDED
#define GraphH_H_INCLUDED

#include <vector>
#include <utility>

typedef unsigned long long ull;
typedef std::pair<int, int> ii;

class Graph{
public:
    ull size;
    std::vector<std::vector<ull> > adjList;
private:
};

#endif
