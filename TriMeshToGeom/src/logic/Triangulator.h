#ifndef Triangulator_H_INCLUDED
#define Triangulator_H_INCLUDED

#include <iostream>
#include <vector>
#include <utility>

#include "data/ObjectCollection.h"
//
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

class Triangulator{
public:
    static vector<vector<int>> triangulate2D(vector<pair<double,double>>& vt, bool positive);
};




#endif // Triangulator_H_INCLUDED
