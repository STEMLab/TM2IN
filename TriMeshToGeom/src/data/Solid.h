#ifndef SOLID_H_INCLUDED
#define SOLID_H_INCLUDED

#include "data/Surface.hpp"


using namespace std;
class Solid{
public:
    Solid(){}
    std::vector<Surface*> surfacesList;

};

#endif // SOLID_H_INCLUDED
