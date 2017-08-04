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


bool Vertex::compare(Vertex* a, Vertex* b){
    if (a->x() < b->x()){
        return true;
    }
    else if (a->x() > b->x()){
        return false;
    }
    else{
        if (a->y() < b->y())
        {
            return true;
        }
        else if (a->y() > b->y())
        {
            return false;
        }
        else{
            if (a->z() < b->z())
            {
                return true;
            }
            else
                return false;

        }
    }
}
