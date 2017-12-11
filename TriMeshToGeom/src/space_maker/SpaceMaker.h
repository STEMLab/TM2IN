#ifndef SPACEMAKER_H_INCLUDED
#define SPACEMAKER_H_INCLUDED

#include "data/Surface.hpp"

class SpaceMaker{
public:
    Checker* checker;
    virtual std::vector<Surface*> makeSimpleSurfaces(std::vector<Surface*> surfacesList) = 0;
};

#endif // SPACEMAKER_H_INCLUDED
