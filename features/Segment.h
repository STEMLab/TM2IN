#ifndef SEGMENT_H_INCLUDED
#define SEGMENT_H_INCLUDED

#include "features/Surface.hpp"

class Segment{
public :
    Vertex* first;
    Vertex* second;

    Segment(){}
    Segment(Vertex* st, Vertex* ed){
        first = st;
        second = ed;
    }

    Vertex* operator[](int idx){
        if (idx == 0) return first;
        else if (idx == 1) return second;
        else assert("idx is 1 0r 0");
        return nullptr;
    }

    std::string toJSON();
    double getSquaredDistance();
    static double compareDistanceIncreased(Segment*, Segment*);
    Segment_2 getCGALSegmentWithoutZ();

};

#endif // SEGMENT_H_INCLUDED
