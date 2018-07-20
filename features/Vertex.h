#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <string>

#include "util.h"
#include "cgal/CGALCalculation.h"
#include "detail/cgal_config.h"

class Vertex{
public:
    ll index = 0;
    double coords[3];

    Vertex():Vertex(0,0,0){}
    Vertex(double px, double py, double pz);
    Vertex(Vertex& vt);
    ~Vertex(){}

    double x(){return coords[0];}
    double y(){return coords[1];}
    double z(){return coords[2];}

    void setX(double value){coords[0] = value;}
    void setY(double value){coords[1] = value;}
    void setZ(double value){coords[2] = value;}
    void setCoords(double x, double y, double z);

    double operator[](int idx){
        return coords[idx];
    }

    void translate(double diff[]);
    void translateTo(std::vector<double> dest);

    std::string asJsonText();
    friend std::ostream& operator<<(std::ostream& ou, Vertex* pVertex);

    Kernel::Point_3 CGAL_point();
};

/** < for Searching upper_bound and lower_bound */
struct CompareVertex_X
{
   bool operator()( const Vertex* v, double x ) const
   {
       return v->coords[0] < x;
   }

   bool operator()( const Vertex* v1, const Vertex* v2 ) const
   {
       return v1->coords[0] < v2->coords[0];
   }

   bool operator()( double time1, double time2 ) const
   {
       return time1 < time2;
   }

   bool operator()( double time, const Vertex* v ) const
   {
      return time < v->coords[0];
   }
};

#endif // VERTEX_H_INCLUDED
