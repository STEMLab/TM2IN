//
// Created by dongmin on 18. 8. 24.
//
#include <sstream>
#include <iomanip>

#include "IndoorGMLWriter.h"
#include "features/RoomBoundary/PolygonMesh.h"
#include "features/Wall/Polygon.h"
#include "features/Vertex.h"
#include "features/Room.h"

namespace TM2IN {
    namespace detail {
        namespace io {
            int IndoorGMLWriter::write(std::vector<Room *> rooms) {
                try {
                    rapidxml::xml_document<> doc;

                    rapidxml::xml_node<> *indoor_features = doc.allocate_node(rapidxml::node_element, "IndoorFeatures");
                    indoor_features->append_attribute(
                            doc.allocate_attribute("xmlns:gml", "http://www.opengis.net/gml/3.2"));
                    indoor_features->append_attribute(
                            doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink"));
                    indoor_features->append_attribute(
                            doc.allocate_attribute("xmlns", "http://www.opengis.net/indoorgml/1.0/core"));
                    indoor_features->append_attribute(
                            doc.allocate_attribute("xmlns:navi", "http://www.opengis.net/indoorgml/1.0/navigation"));
                    indoor_features->append_attribute(
                            doc.allocate_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance"));
                    indoor_features->append_attribute(doc.allocate_attribute("gml:id", "lf1"));
                    indoor_features->append_attribute(doc.allocate_attribute("xsi:schemaLocation",
                                                                             "http://www.opengis.net/indoorgml/1.0/core http://schemas.opengis.net/indoorgml/1.0/indoorgmlcore.xsd http://www.opengis.net/indoorgml/1.0/navigation http://schemas.opengis.net/indoorgml/1.0/indoorgmlnavi.xsd"));
                    doc.append_node(indoor_features);

                    rapidxml::xml_node<> *boundedBy_node = doc.allocate_node(rapidxml::node_element, "gml:boundedBy");
                    boundedBy_node->append_attribute(doc.allocate_attribute("xsi:nil", "true"));
                    indoor_features->append_node(boundedBy_node);

                    rapidxml::xml_node<> *primalSpaceFeatures_node1 = doc.allocate_node(rapidxml::node_element,
                                                                                        "primalSpaceFeatures");
                    indoor_features->append_node(primalSpaceFeatures_node1);

                    rapidxml::xml_node<> *primalSpaceFeatures_node2 = doc.allocate_node(rapidxml::node_element,
                                                                                        "PrimalSpaceFeatures");
                    primalSpaceFeatures_node2->append_attribute(doc.allocate_attribute("gml:id", "psf1"));
                    primalSpaceFeatures_node1->append_node(primalSpaceFeatures_node2);

                    rapidxml::xml_node<> *boundedBy_node2 = doc.allocate_node(rapidxml::node_element, "gml:boundedBy");
                    boundedBy_node2->append_attribute(doc.allocate_attribute("xsi:nil", "true"));
                    primalSpaceFeatures_node2->append_node(boundedBy_node2);

                    rapidxml::xml_node<> *cellSpaceMember_node = doc.allocate_node(rapidxml::node_element,
                                                                                   "cellSpaceMember");
                    primalSpaceFeatures_node2->append_node(cellSpaceMember_node);

                    for (int i = 0; i < rooms.size(); i++) {
                        TM2IN::Room* pRoom = rooms[i];
                        rapidxml::xml_node<>* cell_space_node = doc.allocate_node(rapidxml::node_element, "CellSpace");
                        cell_space_node->append_attribute(doc.allocate_attribute("gml:id", pRoom->name.c_str()));

                        rapidxml::xml_node<>* boundedBy_node_3 = doc.allocate_node(rapidxml::node_element, "gml:boundedBy");
                        boundedBy_node_3->append_attribute(doc.allocate_attribute("xsi:nil", "true"));
                        cell_space_node->append_node(boundedBy_node_3);

                        rapidxml::xml_node<>* cell_space_geometry_node = doc.allocate_node(rapidxml::node_element, "cellSpaceGeometry");
                        cell_space_node->append_node(cell_space_geometry_node);

                        rapidxml::xml_node<>* geometry_3d = doc.allocate_node(rapidxml::node_element, "Geometry3D");
                        cell_space_geometry_node->append_node(geometry_3d);

                        rapidxml::xml_node<>* solid_node = doc.allocate_node(rapidxml::node_element, "gml:Solid");
                        char* solid_name = (char*)pRoom->name.c_str();
                        strcat(solid_name, "_solid");
                        solid_node->append_attribute(doc.allocate_attribute("gml:id", solid_name));
                        geometry_3d->append_node(solid_node);

                        rapidxml::xml_node<>* exterior_node = doc.allocate_node(rapidxml::node_element, "gml:exterior");
                        solid_node->append_node(exterior_node);

                        rapidxml::xml_node<>* shell_node = doc.allocate_node(rapidxml::node_element, "gml:Shell");
                        exterior_node->append_node(shell_node);

                        for (TM2IN::Wall::Polygon* polygon : pRoom->getPm_boundary()->polygons){
                            rapidxml::xml_node<>* surface_member_node = doc.allocate_node(rapidxml::node_element, "gml:surfaceMember");

                            rapidxml::xml_node<>* polygon_node = doc.allocate_node(rapidxml::node_element, "gml:Polygon");

                            rapidxml::xml_node<>* polygon_exterior_node = doc.allocate_node(rapidxml::node_element, "gml:exterior");

                            rapidxml::xml_node<>* linear_ring_node = doc.allocate_node(rapidxml::node_element, "gml:LinearRing");

                            for (Vertex* vt : polygon->getVerticesList()){
                                rapidxml::xml_node<> * pos_node = doc.allocate_node(rapidxml::node_element, "gml:pos");
                                pos_node->append_attribute(doc.allocate_attribute("srsDimension", "3"));

                                char* coord = new char[100];
                                sprintf(coord, "%s %s %s", to_string(vt->x()).c_str(), to_string(vt->y()).c_str(), to_string(vt->z()).c_str());
                                pos_node->value(coord);
                                linear_ring_node->append_node(pos_node);
                            }

                            polygon_exterior_node->append_node(linear_ring_node);
                            polygon_node->append_node(polygon_exterior_node);
                            surface_member_node->append_node(polygon_node);
                            shell_node->append_node(surface_member_node);
                        }
                        cellSpaceMember_node->append_node(cell_space_node);
                    }

                    // rapidxml::print(std::back_inserter(xml_as_string), doc);

                    // Save to file
                    file_stored << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
                    file_stored << doc;
                    // file_stored << xml_as_string;
                    file_stored.close();
                    doc.clear();
                }
                catch(std::exception& e){
                    cerr << e.what() << endl;
                }

                return 0;
            }


        }
    }
}