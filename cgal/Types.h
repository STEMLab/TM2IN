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
#include <CGAL/linear_least_squares_fitting_3.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Plane_3 Plane_3;
typedef CGAL::Aff_transformation_3<Kernel> Transformation;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Triangle_3 Triangle_3;
typedef Kernel::Line_3 Line_3;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Intersect_2 Intersect_2;

#endif //TRIANGLEMESHTOCLEARSOLID_CGALTYPES_H
