#include <string>
#include <cgal/Features_to_CGAL_object.h>
#include <detail/feature/type_conversion.h>

#include "features/Vertex.h"
#include "detail/io/JsonWriter.h"

using namespace std;

std::string Vertex::asJsonText()
{
    return TM2IN::detail::io::to_json(*this);
}

void Vertex::translate(double diff[]){
    for (int i = 0 ; i < 3 ; i++) coords[i] += diff[i];
}

void Vertex::translateTo(vector<double> dest){
    for (int i = 0 ; i < 3; i++) coords[i] = dest[i];
}

void Vertex::setIndex(unsigned long i) {
    this->index = i;
}

Kernel::Point_3 Vertex::CGAL_point() {
    return TM2IN::detail::feature::to_CGAL_Point_3(*this);
}
