//
// Created by dongmin on 18. 7. 18.
//

#include "JsonWriter.h"

#include "features/Wall/TriangulatedSurface.h"
#include "features/Room.h"
#include "features/RoomBoundary/PolygonMesh.h"
#include "features/RoomBoundary/TriangleMesh.h"
#include "features/RoomBoundary/TriangulatedSurfaceMesh.h"
#include "features/Vertex.h"
#include "features/Wall/Triangle.h"
#include "features/Wall/Polygon.h"

namespace TM2IN {
    namespace detail {
        namespace io {
            JsonWriter::JsonWriter(std::ofstream &_fout) : fout(_fout){}

            void JsonWriter::write(vector<Room *> &rooms, int boundary_mode) {
                fout << "{ \n";
                fout << " \"spaces\" : [ \n";
                for (unsigned int index = 0; index < rooms.size(); index++) {
                    string ts_str = TM2IN::detail::io::room_to_json(*rooms[index], boundary_mode);
                    fout << ts_str;
                    if (index != rooms.size() - 1) fout << ", \n";
                    else fout << "\n";
                }
                fout << "]" << endl;
                fout << "}" << endl;
            }

            string room_to_json(Room& room, int boundary_mode){
                string result;
                result = "{\n";
                result += " \"name\" : \"" + room.name + "\", \n";
                result += " \"Surfaces\" : [ \n";
                if (boundary_mode == 0) // PM
                {
                    RoomBoundary::PolygonMesh* pm = room.getPm_boundary();
                    vector<Wall::Polygon*>& polygons = pm->polygons;
                    for (unsigned int id = 0; id < polygons.size(); id++) {
                            result += surface_to_json(*(polygons[id]));

                        if (id != polygons.size() - 1) {
                            result += ", \n";
                        } else {
                            result += " \n";
                        }
                    }
                }
                else if (boundary_mode == 1) // TM
                {
                    RoomBoundary::TriangleMesh* tm = room.getTm_boundary();
                    vector<Wall::Triangle*> triangles = tm->getTriangleList();
                    for (unsigned int id = 0; id < triangles.size(); id++) {
                        result += surface_to_json(*(triangles[id]));

                        if (id != triangles.size() - 1) {
                            result += ", \n";
                        } else {
                            result += " \n";
                        }
                    }
                }
                else if (boundary_mode == 2){ // TSM and with Triangle
                    RoomBoundary::TriangulatedSurfaceMesh* tsm = room.getTsm_boundary();
                    for (unsigned int id = 0; id < tsm->surfaces.size(); id++) {
                        result += surface_to_json_with_triangles(*(tsm->surfaces[id]));
                        if (id != tsm->surfaces.size() - 1) {
                            result += ", \n";
                        } else {
                            result += " \n";
                        }
                    }
                }
                else{ // TSM
                    RoomBoundary::TriangulatedSurfaceMesh* tsm = room.getTsm_boundary();
                    for (unsigned int id = 0; id < tsm->surfaces.size(); id++) {
                        result += surface_to_json(*(tsm->surfaces[id]));
                        if (id != tsm->surfaces.size() - 1) {
                            result += ", \n";
                        } else {
                            result += " \n";
                        }
                    }
                }

                result += "] \n";
                result += "}";
                return result;
            }

            string surface_to_json_with_triangles(Wall::TriangulatedSurface& pSurface) {
                assert(pSurface.triangles.size() > 0);

                string ret;
                ret += "{";
                ret.append(" \n \"area\" : " + to_string(pSurface.getArea()) );
                ret.append(" \n, \"id\" : \"" + pSurface.geom_id + "\"" );
                ret.append( "\n, \"triangles\" : [\n");
                for (int i = 0 ; i < (int)pSurface.triangles.size() ; i++){
                    ret += pSurface.triangles[i]->asJsonText();
                    if (i != pSurface.triangles.size() - 1)
                        ret += ",";
                    ret += "\n";
                }
                ret.append( "]\n");
                ret.append( "}");

                return ret;
            }

            string surface_to_json(Wall::Surface& sf){
                if (sf.getVerticesSize() == 0){
                    throw std::runtime_error("string to_json(Surface) : Vertex size is 0");
                }
                string ret;
                ret.append("{");
                ret.append(" \n \"area\" : " + to_string(sf.getArea()) );
                ret.append(" ,\n \"id\" : \"" + sf.geom_id + "\"" );
                ret.append(" ,\n \"normal\" : [");
                ret.append(to_string(sf.normal.x()) + ", ");
                ret.append(to_string(sf.normal.y()) + ", ");
                ret.append(to_string(sf.normal.z()));
                ret.append("], \n");
                ret.append(" \"coord\" : [");
                for (unsigned int i = 0 ; i < sf.getVerticesSize() ; i++){
                    ret.append(sf.vertex(i)->asJsonText());
                    ret.append(",");
                }
                ret.append(sf.vertex(0)->asJsonText());
                ret.append("] }");
                return ret;
            }

            string vertex_to_json(Vertex& vt, int index){
                std::string ret;
                ret.append("[");
                ret.append(to_string(vt.x()));
                ret.append(",");
                ret.append(to_string(vt.y()));
                ret.append(",");
                ret.append(to_string(vt.z()));
                ret.append(",");
                ret.append(to_string(index));
                ret.append("]");
                return ret;
            }
        }
    }
}
