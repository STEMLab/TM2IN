#include "Triangulator.h"

vector<vector<int>> Triangulator::triangulate(Surface* sf){
//    vector<pair<Point, unsigned>> points;
//    for (int i = 0 ; i < (int)sf->v_list.size() ; i++){
//        points.push_back(make_pair(makePoint(sf->v_list[i]),i));
//    }
//
//    Delaunay triangulation;
//    triangulation.insert(points.begin(), points.end());
//
//    for(Delaunay::Finite_faces_iterator fit = triangulation.finite_faces_begin();
//    fit != triangulation.finite_faces_end(); ++fit)
//    {
//        Delaunay::Face_handle face = fit;
//        std::cout << "Triangle:\t" << triangulation.triangle(face) << std::endl;
//        std::cout << "Vertex 0:\t" << triangulation.triangle(face)[0] << std::endl;
//        std::cout << "Vertex 0:\t" << face->vertex(0)->info() << std::endl;
//    }

}
//
//Point Triangulator::makePoint(Vertex* vt){
//    return Point(vt->x(), vt->y(), vt->z());
//}
