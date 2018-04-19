#include <string>

#include "features/Vertex.h"

using namespace std;

std::string Vertex::toJSONString()
{
    std::string ret;
    ret.append("[");
    ret.append(to_string(x()));
    ret.append(",");
    ret.append(to_string(y()));
    ret.append(",");
    ret.append(to_string(z()));
    ret.append(",");
    ret.append(to_string(this->index));
    ret.append("]");
    return ret;
}

void Vertex::translate(double diff[]){
    for (int i = 0 ; i < 3 ; i++) coords[i] += diff[i];
}

void Vertex::translateTo(vector<double> diff){
    for (int i = 0 ; i < 3; i++) coords[i] = diff[i];
}

void Vertex::setIndex(unsigned long i) {
    this->index = i;
}
