//
// Created by dongmin on 18. 1. 17.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_CGALTYPES_H
#define TRIANGLEMESHTOCLEARSOLID_CGALTYPES_H


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Kernel/global_functions.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>
#include <CGAL/Point_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/enum.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/intersections.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Simple_cartesian<double> Kernel;
//typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Aff_transformation_3<Kernel> Transformation;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Plane_3 Plane_3;
typedef Kernel::Triangle_3 Triangle_3;
typedef Kernel::Line_3 Line_3;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Intersect_2 Intersect_2;
typedef Kernel::Intersect_3 Intersect_3;

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<double, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             D_Point;
#endif //TRIANGLEMESHTOCLEARSOLID_CGALTYPES_H
