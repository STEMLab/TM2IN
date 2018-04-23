//
// Created by dongmin on 18. 4. 23.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_POLYGONIZER_H
#define TRIANGLEMESHTOCLEARSOLID_POLYGONIZER_H


#include <features/Space.h>

class Polygonizer {
public:
    virtual void make(Space* space) = 0;
};


#endif //TRIANGLEMESHTOCLEARSOLID_POLYGONIZER_H
