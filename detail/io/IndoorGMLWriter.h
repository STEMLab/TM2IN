//
// Created by dongmin on 18. 8. 24.
//

#ifndef TM2IN_INDOORGMLWRITER_H
#define TM2IN_INDOORGMLWRITER_H

#include "lib/rapidxml/rapidxml.hpp"
#include "lib/rapidxml/rapidxml_print.hpp"
#include "lib/rapidxml/rapidxml_utils.hpp"
#include "features/IndoorComponent.h"

#include <fstream>

using namespace std;

namespace TM2IN {
    namespace detail {
        namespace io {
            /**
             * @ingroup imp_details
             */
            class IndoorGMLWriter {
            public:
                IndoorGMLWriter(ofstream& fout) : file_stored(fout) {}
                int write(std::vector<Room*> rooms);
            private:
                ofstream& file_stored;

                rapidxml::xml_node<> *create_cell_space_node(Room *pRoom, rapidxml::xml_document<>& document);

                rapidxml::xml_node<> *
                create_surface_member_node(Wall::Polygon *pPolygon, rapidxml::xml_document<> &document);

            };
        }
    }
}

#endif //TM2IN_INDOORGMLWRITER_H
