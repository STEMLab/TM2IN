#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <vector>

// Our vertex type
typedef struct{
    float x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
    int a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
    float u,v;
}mapcoord_type;

class CombinedPolygon;
class obj_type;
class Checker;
class OBJCollection;
class TVRCollection;


#endif // MODEL_H_INCLUDED
