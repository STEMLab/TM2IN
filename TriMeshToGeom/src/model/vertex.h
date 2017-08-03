#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <string>

#include "logic/util.h"

class Vertex{
public:
    ll index = 0;
    double x = 0.0,y =0.0 ,z = 0.0;
    Vertex(double px, double py, double pz){
        x = px;
        y = py;
        z = pz;
    }

    Vertex(){
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    Vertex(Vertex& vt){
        this->x = vt.x;
        this->y = vt.y;
        this->z = vt.z;
        this->index = vt.index;
    }

    std::string toJSON();
};



#endif // VERTEX_H_INCLUDED
