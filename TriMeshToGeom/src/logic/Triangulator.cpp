#include "Triangulator.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_3<double, K>    Vb3D;
typedef CGAL::Triangulation_data_structure_3<Vb3D>                    Tds3D;
typedef CGAL::Delaunay_triangulation_3<K, Tds3D, CGAL::Fast_location> Delaunay3D;
typedef Delaunay3D::Point                                             Point3D;

typedef CGAL::Triangulation_vertex_base_with_info_2<double, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             Point2D;


vector<vector<int>> Triangulator::triangulate2D(vector<Vertex*> vt_list, int type){
    vector<vector<int>> triangulation_index;
    vector<pair<Point2D, unsigned>> points;
    for (int i = 0 ; i < vt_list.size() ; i++){
        int x_i = (type + 1) % 3;
        double x = (*vt_list[i])[x_i];
        double y = (*vt_list[i])[(type+2) % 3];
        points.push_back(make_pair(Point2D(x,y), i));
    }

    Delaunay triangulation;
    triangulation.insert(points.begin(), points.end());

    CGAL_assertion( triangulation.number_of_vertices() == vt_list.size() );

    for(Delaunay::Finite_faces_iterator fit = triangulation.finite_faces_begin();
    fit != triangulation.finite_faces_end(); ++fit)
    {
        vector<int> temp;
        Delaunay::Face_handle face = fit;
        if (type < 3)
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

vector<vector<int>> Triangulator::triangulate(Surface* sf){
    vector<vector<int>> triangulation_index;
    vector<pair<Point3D, unsigned>> points;
    for (int i = 0 ; i < (int)sf->v_list.size() ; i++){
        Vertex* vt = sf->v_list[i];
        points.push_back(make_pair(Point3D(vt->x(), vt->y(), vt->z()), i) );
    }

    Delaunay3D triangulation;
    triangulation.insert(points.begin(), points.end());

    CGAL_assertion( triangulation.number_of_vertices() == sf->v_list.size() );

    int face_num = 0;
    for(Delaunay3D::Finite_cells_iterator fit = triangulation.finite_cells_begin();
    fit != triangulation.finite_cells_end(); ++fit)
    {
        vector<int> temp;
        Delaunay3D::Cell_handle cell = fit;
        for (int i = 0 ; i < 4 ; i++){
            Delaunay3D::Vertex_handle vertex = cell->vertex(i);
            temp.push_back(vertex->info());
        }
        triangulation_index.push_back(temp);
        face_num++;

    }
    return triangulation_index;
}
//
//Point3D Triangulator::makePoint3D(Vertex* vt){
//    return Point3D(vt->x(), vt->y(), vt->z());
//}
