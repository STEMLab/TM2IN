#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <string>

#include "logic/util.h"

class Vertex{
public:
    ll index = 0;
    double coords[3];
    Vertex(double px, double py, double pz){
        coords[0] = px;
        coords[1] = py;
        coords[2] = pz;
    }

    Vertex(){
        coords[0] = 0.0;
        coords[1] = 0.0;
        coords[2] = 0.0;
    }

    Vertex(Vertex& vt){
        coords[0] = vt.x();
        coords[1] = vt.y();
        coords[2] = vt.z();
        this->index = vt.index;
    }

    double x(){return coords[0];}
    double y(){return coords[1];}
    double z(){return coords[2];}

    void setX(double value){coords[0] = value;}
    void setY(double value){coords[1] = value;}
    void setZ(double value){coords[2] = value;}

    double operator[](int idx){
        return coords[idx];
    }

    std::string toJSON();
    static bool compare(Vertex*,Vertex*);
};



#endif // VERTEX_H_INCLUDED
