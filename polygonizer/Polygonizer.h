//
// Created by dongmin on 18. 4. 23.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_POLYGONIZER_H
#define TRIANGLEMESHTOCLEARSOLID_POLYGONIZER_H


#include <features/PolyhedralSurface.h>

class Polygonizer {
public:
    virtual void make(PolyhedralSurface* space) = 0;
};


#endif //TRIANGLEMESHTOCLEARSOLID_POLYGONIZER_H
