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

    Vertex(double px, double py, double pz){
        coords[0] = CGAL::to_double(px);
        coords[1] = CGAL::to_double(py);
        coords[2] = CGAL::to_double(pz);
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

    ~Vertex(){
        //delete[] coords;
    }

    double x(){return coords[0];}
    double y(){return coords[1];}
    double z(){return coords[2];}

    void setX(double value){coords[0] = value;}
    void setY(double value){coords[1] = value;}
    void setZ(double value){coords[2] = value;}
    void setCoords(double x, double y, double z){
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }

    double operator[](int idx){
        return coords[idx];
    }

    void translate(double diff[]);
    void translateTo(std::vector<double> dest);

    std::string asJsonText();
    friend std::ostream& operator<<(std::ostream& ou, Vertex* pVertex){
        ou << pVertex->asJsonText() << std::endl;
        return ou;
    }
    void setIndex(unsigned long i);

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
