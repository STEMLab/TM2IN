//
// Created by dongmin on 18. 7. 16.
//

#include "TVRReader.h"

#include "features/TriangleMesh.h"
#include "features/Vertex.h"
#include "features/Triangle.h"

namespace TM2IN {
    namespace detail {
        namespace io {
            TVRReader::TVRReader(std::ifstream &_ifs): ifs(_ifs) {

            }

            std::vector<TriangleMesh *> TVRReader::read() {
                string inputstr;
                getline(this->ifs, inputstr);
                if (inputstr.find("TVR0") == string::npos){
                    cout << "different version : " <<  inputstr << endl;
                    return vector<TriangleMesh*>();
                }

                string group_name;
                int v_count = 0;
                int f_count = 0;

                vector<TriangleMesh*> meshList;
                TriangleMesh* currentMesh = new TriangleMesh();

                vector<Vertex*> vertices;

                while(!this->ifs.eof()){
                    getline(this->ifs, inputstr);
                    if (inputstr.size() < 3) continue;
                    switch(inputstr.c_str()[0]){
                        case 'c':{
                            break;
                        }
                        case 'v':{
                            Vertex* pt_v = this->makeVertex(v_count, inputstr);
                            vertices.push_back(pt_v);

                            v_count++;
                            if (v_count % 5000 == 0) cout << "Loaded vertices : " << v_count << endl;

                            break;
                        }
                        case 'g':{
                            if (f_count != 0){
                                currentMesh->vertices = vertices;
                                meshList.push_back(currentMesh);
                                currentMesh = new TriangleMesh();
                                f_count = 0;
                            }
                            group_name = this->getGroupName(inputstr);
                            if (group_name.find('\r') != string::npos) group_name.erase(group_name.find('\r'));
                            currentMesh->name = group_name;
                            break;
                        }
                        case 'f':{
                            assert(vertices.size() > 0);

                            f_count++;
                            if (f_count % 5000 == 0) cout << "Loaded faces : " << f_count << endl;

                            Triangle* tri = this->makeTriangle(inputstr, vertices);
                            currentMesh->triangles.push_back(tri);

                            break;
                        }
                    }
                }
                currentMesh->vertices = vertices;
                meshList.push_back(currentMesh);
                return meshList;
            }


            string TVRReader::getGroupName(string& input){
                std::vector<std::string> x = split(input, ' ');
                string x_1(x[1]);
                x.clear();
                return x_1;
            }

            Triangle* TVRReader::makeTriangle(string& input, vector<Vertex*>& vertex){
                std::vector<std::string> x = split(input, ' ');

                ll a = stol(x[1]);
                ll b = stol(x[2]);
                ll c = stol(x[3]);

                Triangle* newTriangle = new Triangle(vertex[a], vertex[b], vertex[c]);

                x.clear();
                return newTriangle;
            }

            Vertex* TVRReader::makeVertex(int id, string &input){
                std::stringstream ss;
                ss.str(input);

                string line;
                getline(ss, line, '\t');
                std::vector<std::string> strings = split(line, ' ');

                Vertex* vt = new Vertex(stod(strings[1]), stod(strings[2]), stod(strings[3]));
                vt->index = id;
                strings.clear();

                return vt;
            }
        }
    }
}