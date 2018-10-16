#include <string>
#include <detail/cgal/type_conversion.h>

#include "features/Vertex.h"
#include "detail/io/JsonWriter.h"

using namespace std;

namespace TM2IN {
    std::string Vertex::asJsonText() {
        return TM2IN::detail::io::vertex_to_json(*this, this->geom_id);
    }

    void Vertex::translate(double diff[]) {
        for (int i = 0; i < 3; i++) coords[i] += diff[i];
    }

    void Vertex::translateTo(vector<double> dest) {
        for (int i = 0; i < 3; i++) coords[i] = dest[i];
    }


    Kernel::Point_3 Vertex::CGAL_point() {
        return TM2IN::detail::cgal::to_CGAL_Point_3(*this);
    }

    void Vertex::setCoords(double x, double y, double z) {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }

    std::ostream &operator<<(std::ostream &ou, Vertex *pVertex) {
        ou << pVertex->asJsonText() << std::endl;
        return ou;
    }

    std::string Vertex::asWKT() {
        return TM2IN::detail::io::vertex_to_wkt(this);
    }
}