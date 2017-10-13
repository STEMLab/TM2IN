#include "model/Segment.h"


std::string Segment::toJSON(){
    std::string ret;
    ret += first->toJSON();
    ret += " --> ";
    ret += second->toJSON();
    return ret;
}
