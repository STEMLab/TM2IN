#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <string>

#include "util.h"
#include "detail/cgal_config.h"
#include "IndoorComponent.h"

namespace TM2IN {
    /**
     * @ingroup geo_features
     * @brief Vertex imply one coordinate in 3-D indoor space.
     */
    class Vertex : public IndoorComponent {
    public:
        double coords[3];

        /**
         * @brief Empty Vertex constructor
         */
        Vertex() : Vertex(0, 0, 0) {}

        /**
         * @brief Constructor with three double values
         */
        Vertex(double px, double py, double pz) {
            coords[0] = CGAL::to_double(px);
            coords[1] = CGAL::to_double(py);
            coords[2] = CGAL::to_double(pz);
            type = TM2IN::IND_TYPE::Vertex;
        }

        /**
         * @brief Copy constructor.
         */
        Vertex(Vertex &vt) : Vertex(vt.x(), vt.y(), vt.z()) {  }

        /**
         * @brief Destructor
         */
        ~Vertex() {}

        /**
         * @brief Gets x value.
         */
        double x() { return coords[0]; }

        /**
         * @brief Gets y value
         */
        double y() { return coords[1]; }

        /**
         * @brief Gets z value
         */
        double z() { return coords[2]; }

        /**
         * @brief Sets x value
         *
         */
        void setX(double value) { coords[0] = value; }

        /**
         * @brief Sets y value
         */
        void setY(double value) { coords[1] = value; }

        /**
         * @brief Sets z value
         */
        void setZ(double value) { coords[2] = value; }

        /**
         * @brief Sets coordinate with three float values.
         */
        void setCoords(double x, double y, double z);

        /**
         * @brief Operator [] to access each value
         */
        double operator[](int idx) {
            return coords[idx];
        }

        /**
         * @brief Translate coordinate values with difference
         */
        void translate(double diff[]);

        /**
         * @brief Move Vertex to destination
         */
        void translateTo(std::vector<double> dest);

        std::string asJsonText();
        std::string asWKT();


        /**
         * @brief Operator << : print string
         */
        friend std::ostream &operator<<(std::ostream &ou, Vertex *pVertex);

        /**
         * @brief Converts to Kernel::Point_3
         */
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
}


#endif // VERTEX_H_INCLUDED
