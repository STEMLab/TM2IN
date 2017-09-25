#ifndef SOLID_H_INCLUDED
#define SOLID_H_INCLUDED

#include "data/Surface.hpp"


using namespace std;
class Solid{
public:
    Solid(){}

    void makeSimpleSolid(vector<Surface*> sf_list);
};

#endif // SOLID_H_INCLUDED
