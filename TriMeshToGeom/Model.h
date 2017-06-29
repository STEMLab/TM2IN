#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <vector>

// Our vertex type
class vertex_type{
public:
    double x = 0.0,y =0.0 ,z = 0.0;
    vertex_type(double px, double py, double pz){
        x = px;
        y = py;
        z = pz;
    }
    vertex_type(){
    }
};
//typedef struct {
//    double x,y,z;
//} vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
    unsigned long a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
    float u,v;
}mapcoord_type;

//class CombinedPolygon;
class obj_type;
class Checker;
class OBJCollection;
class TVRCollection;


#endif // MODEL_H_INCLUDED
