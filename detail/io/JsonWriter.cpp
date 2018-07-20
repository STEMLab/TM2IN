//
// Created by dongmin on 18. 7. 18.
//

#include "JsonWriter.h"

namespace TM2IN {
    namespace detail {
        namespace io {
            JsonWriter::JsonWriter(std::ofstream &_fout) : fout(_fout){}

            void JsonWriter::write(vector<PolyhedralSurface *> &ts, bool exp_tri) {
                fout << "{ \n";
                fout << " \"spaces\" : [ \n";
                for (unsigned int index = 0; index < ts.size(); index++) {
                    string ts_str = TM2IN::detail::io::to_json(ts[index], exp_tri);
                    fout << ts_str;
                    if (index != ts.size() - 1) fout << ", \n";
                    else fout << "\n";
                }
                fout << "]" << endl;
                fout << "}" << endl;
            }

            string to_json(PolyhedralSurface* ps, bool exp_tri){
                string result;
                result = "{\n";
                result += " \"name\" : \"" + ps->name + "\", \n";
                result += " \"Surfaces\" : [ \n";
                for (unsigned int id = 0; id < ps->surfacesList.size(); id++) {
                    if (exp_tri)
                        result += to_json_with_triangles(ps->surfacesList[id]);
                    else
                        result += to_json(ps->surfacesList[id]);

                    if (id != ps->surfacesList.size() - 1) {
                        result += ", \n";
                    } else {
                        result += " \n";
                    }
                }
                result += "] \n";
                result += "}";
                return result;
            }

            string to_json_with_triangles(Surface* pSurface) {
                assert(pSurface->triangles.size() > 0);

                string ret;
                ret += "{";
                ret.append(" \n \"area\" : " + to_string(pSurface->area) );
                ret.append(" \n, \"id\" : \"" + pSurface->sf_id + "\"" );
                ret.append( "\n, \"triangles\" : [\n");
                for (int i = 0 ; i < (int)pSurface->triangles.size() ; i++){
                    ret += pSurface->triangles[i]->asJsonText();
                    if (i != pSurface->triangles.size() - 1)
                        ret += ",";
                    ret += "\n";
                }
                ret.append( "]\n");
                ret.append( "}");

                return ret;
            }

            string to_json(Surface* sf){
                if (sf->getVerticesSize() == 0){
                    throw std::runtime_error("string to_json(Surface) : Vertex size is 0");
                }
                string ret;
                ret.append("{");
                ret.append(" \n \"area\" : " + to_string(sf->area) );
                ret.append(" ,\n \"id\" : \"" + sf->sf_id + "\"" );
                ret.append(" ,\n \"normal\" : [");
                ret.append(to_string(sf->normal.x()) + ", ");
                ret.append(to_string(sf->normal.y()) + ", ");
                ret.append(to_string(sf->normal.z()));
                ret.append("], \n");
                ret.append(" \"coord\" : [");
                for (unsigned int i = 0 ; i < sf->getVerticesSize() ; i++){
                    ret.append(sf->vertex(i)->asJsonText());
                    ret.append(",");
                }
                ret.append(sf->vertex(0)->asJsonText());
                ret.append("] }");
                return ret;
            }
/*
            string to_json(Triangle* tri){
                std::string ret;
                ret += "{\n";

                //normal
                ret += "\"normal\" : [";
                Vector_3 normal = tri->getNormal();
                ret += std::to_string(normal.x()) + ", " + std::to_string(normal.y()) + ", " + std::to_string(normal.z());
                ret += "],\n";

                //area
                ret += "\"area\" : ";
                double area = tri->getArea();
                ret += std::to_string(area);
                ret += "\n";

                //coordinates
                ret += "\"coord\" : [\n";
                for (int i = 0 ; i < 3 ; i++){
                    ret += "\t";
                    ret += tri->vertex(i)->asJsonText();
                    if (i != 2)
                        ret += ",";
                    ret += "\n";
                }
                ret += "]}";

                return ret;
            }
*/
            string to_json(Vertex& vt){
                std::string ret;
                ret.append("[");
                ret.append(to_string(vt.x()));
                ret.append(",");
                ret.append(to_string(vt.y()));
                ret.append(",");
                ret.append(to_string(vt.z()));
                ret.append(",");
                ret.append(to_string(vt.index));
                ret.append("]");
                return ret;
            }
        }
    }
}
