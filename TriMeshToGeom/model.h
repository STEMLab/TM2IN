#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#define MAX_VERTICES 16000 // Max number of vertices (for each object)
#define MAX_POLYGONS 16000 // Max number of polygons (for each object)

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

// The object type
typedef struct {
	char name[20];

	int vertices_qty;
    int polygons_qty;

    vertex_type* vertex;
    polygon_type* polygon;
} obj_type, *obj_type_ptr;


#endif // MODEL_H_INCLUDED
