#ifndef CleanPolygonMaker_H_INCLUDED
#define CleanPolygonMaker_H_INCLUDED

#include <vector>

#include "data/Surface.hpp"

using namespace std;
class CleanPolygonMaker{
public:
    static bool combine(Surface* cp1, Surface* cp2, Checker* checker);
    static bool isNeighbor(Surface* cp1, Surface* cp2);
    static void findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j);
    static int simplifyShareLine(Surface* , Surface*);
    static bool findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j);
};

#endif // CleanPolygonMaker_H_INCLUDED
