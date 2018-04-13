//
// Created by dongmin on 18. 4. 10.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_FEATURES_TO_CGAL_OBJECT_H
#define TRIANGLEMESHTOCLEARSOLID_FEATURES_TO_CGAL_OBJECT_H

#include <iostream>
#include <vector>
#include "predefine.h"
#include "Types.h"
#include "features/Vertex.h"
#include "features/Triangle.h"


namespace CGAL_User {
    Point_3 getCGALPoint(Vertex* vt);
    Triangle_3 getCGALTriangle(Triangle*& pTriangle);
}

#endif //TRIANGLEMESHTOCLEARSOLID_FEATURES_TO_CGAL_OBJECT_H
