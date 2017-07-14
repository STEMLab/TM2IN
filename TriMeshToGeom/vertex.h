#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <string>

class Vertex{
public:
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

    std::string toJSON();
};



#endif // VERTEX_H_INCLUDED
