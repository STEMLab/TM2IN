#include "Triangulator.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<double, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             Point2D;


vector<vector<int>> Triangulator::triangulate2D(vector<pair<double, double>>& vt_list, bool positive){
    vector<vector<int>> triangulation_index;
    vector<pair<Point2D, unsigned>> points;
    for (int i = 0 ; i < (int)vt_list.size() ; i++){
        double x = vt_list[i].first;
        double y = vt_list[i].second;
        points.push_back(make_pair(Point2D(x,y), i));
        cout << x << " " << y << endl;
    }

    Delaunay triangulation;
    triangulation.insert(points.begin(), points.end());

    if( triangulation.number_of_vertices() != vt_list.size() ){
        return triangulation_index;
    }


    for(Delaunay::Finite_faces_iterator fit = triangulation.finite_faces_begin();
    fit != triangulation.finite_faces_end(); ++fit)
    {
        vector<int> temp;
        Delaunay::Face_handle face = fit;
        if (true)
        {
            for (int i = 0 ; i < 3 ; i++){
                temp.push_back(face->vertex(i)->info());
            }
        }
        else{
            for (int i = 2 ; i >= 0 ; i--){
                temp.push_back(face->vertex(i)->info());
            }
        }

        triangulation_index.push_back(temp);

    }

    if (triangulation_index.size() == 0){
        cout << "triangulate error" << endl;
    }
    return triangulation_index;
}
