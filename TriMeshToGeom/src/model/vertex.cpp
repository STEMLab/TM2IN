#include <string>

#include "model/vertex.h"

using namespace std;

std::string Vertex::toJSON()
{
    std::string ret;
    ret.append("[");
    ret.append(to_string(x()));
    ret.append(",");
    ret.append(to_string(y()));
    ret.append(",");
    ret.append(to_string(z()));
    ret.append("]");
    return ret;
}
