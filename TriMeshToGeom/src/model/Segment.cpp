#include "model/Segment.h"
#include "logic/CGALCalculation.h"

std::string Segment::toJSON(){
    std::string ret;
    ret += first->toJSON();
    ret += " --> ";
    ret += second->toJSON();
    return ret;
}

double Segment::getSquaredDistance(){
    return CGALCalculation::getSquaredDistance(this->first, this->second);
}

double Segment::compareDistanceIncreased(Segment* s_i, Segment* s_j){
    return s_i->getSquaredDistance() < s_j->getSquaredDistance();
}

Segment_2 Segment::getCGALSegmentWithoutZ(){
    Point_2 p1(this->first->x(), this->first->y());
    Point_2 p2(this->second->x(), this->second->y());

    return Segment_2(p1, p2);
}
