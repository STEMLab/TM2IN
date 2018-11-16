//
// Created by dongmin on 18. 7. 16.
//

#include "TVRReader.h"

#include "features/Room.h"
#include "detail/features/RoomFactory.h"
#include "features/Vertex.h"
#include "features/Wall/Triangle.h"

namespace TM2IN {
    namespace detail {
        namespace io {
            TVRReader::TVRReader(std::ifstream &_ifs): ifs(_ifs) {

            }

            std::vector<Room*> TVRReader::read() {
                int version = 0;
                string inputstr;
                getline(this->ifs, inputstr);
                if (inputstr.find("TVR0") != string::npos){
                    version = 0;
                }
                else if (inputstr.find("TVR1") != string::npos){
                    version = 1;
                }
                else{
                    std::runtime_error("different version : " + inputstr);
                }
                cout << "TVR Version  : " << version << endl;

                int v_count = 0;
                int f_count = 0;

                vector<Room*> rooms;
                RoomFactory factory;

                if (version == 0)
                    factory.keep_vertices = true;
                else
                    factory.keep_vertices = false;

                bool is_one_room = (version == 1);
                ull pre_vertex = 0;

                while(!this->ifs.eof()){
                    getline(this->ifs, inputstr);
                    if (inputstr.size() < 3) continue;
                    switch(inputstr.c_str()[0]){
                        case 'c':{
                            break;
                        }
                        case 'v':{
                            auto pt_v = this->makeVertex(inputstr);
                            factory.pushVertex(pt_v);

                            v_count++;
                            if (v_count % 5000 == 0) cout << "Loaded vertices : " << v_count << endl;

                            break;
                        }
                        case 'g':{
                            if (is_one_room){
                                if (f_count != 0){
                                    pre_vertex += v_count;
                                }
                                else{
                                    string group_name = this->getGroupName(inputstr);
                                    if (group_name.find('\r') != string::npos) group_name.erase(group_name.find('\r'));
                                    factory.setRoomName(group_name);
                                }
                            }
                            else{
                                if (f_count != 0){
                                    cout << "Building Triangle Adjacency relation" << f_count << endl;
                                    factory.buildStrTriangle();
                                    vector<Room*> curr_rooms = factory.make();
                                    if (curr_rooms.size() != 0)
                                        rooms.insert(rooms.end(), curr_rooms.begin(), curr_rooms.end());
                                    pre_vertex += v_count;
                                    f_count = 0;
                                }
                                string group_name = this->getGroupName(inputstr);
                                if (group_name.find('\r') != string::npos) group_name.erase(group_name.find('\r'));
                                factory.setRoomName(group_name);
                            }
                            break;
                        }
                        case 'f':{
                            assert(v_count != 0);

                            //Wall::Triangle* tri = this->makeTriangle(inputstr, factory.getVerticesList());
                            //factory.pushTriangle(tri);

                            std::vector<std::string> x = split(inputstr, ' ');

                            ll a = stol(x[1]) + pre_vertex;
                            ll b = stol(x[2]) + pre_vertex;
                            ll c = stol(x[3]) + pre_vertex;
                            factory.pushTriangle(a, b, c);

                            x.clear();

                            f_count++;
                            if (f_count % 5000 == 0) cout << "Loaded faces : " << f_count << endl;

                            break;
                        }
                    }
                }
                cout << "Building Triangle Adjacency relation... " << f_count << endl;
                factory.buildStrTriangle();

                vector<Room*> curr_rooms = factory.make();
                if (curr_rooms.size() != 0)
                    rooms.insert(rooms.end(), curr_rooms.begin(), curr_rooms.end());
                return rooms;
            }

            string TVRReader::getGroupName(string& input){
                std::vector<std::string> x = split(input, ' ');
                string x_1(x[1]);
                x.clear();
                return x_1;
            }

            Wall::Triangle* TVRReader::makeTriangle(string& input, vector<TM2IN::Vertex*>& vertex){
                std::vector<std::string> x = split(input, ' ');

                ll a = stol(x[1]);
                ll b = stol(x[2]);
                ll c = stol(x[3]);

                auto va = vertex[a];
                auto vb = vertex[b];
                auto vc = vertex[c];
                Wall::Triangle* newTriangle = new Wall::Triangle(va, vb, vc);

                x.clear();
                return newTriangle;
            }

            Vertex * TVRReader::makeVertex(string &input) {
                std::stringstream ss;
                ss.str(input);

                string line;
                getline(ss, line, '\t');
                std::vector<std::string> strings = split(line, ' ');

                auto vt = new Vertex(stod(strings[1]), stod(strings[2]), stod(strings[3]));
                strings.clear();

                return vt;
            }
        }
    }
}