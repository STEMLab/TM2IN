#ifndef Triangulator_H_INCLUDED
#define Triangulator_H_INCLUDED

#include <iostream>
#include <vector>
#include <utility>

#include "data/ObjectCollection.h"
//
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

class Triangulator{
public:
    static vector<vector<int>> triangulate(Surface* sf);
    static vector<vector<int>> triangulate2D(vector<Vertex*> vt, int type);
};




#endif // Triangulator_H_INCLUDED
