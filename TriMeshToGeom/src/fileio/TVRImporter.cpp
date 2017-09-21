#include "TVRImporter.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include "data/Space.h"
#include "model/vertex.h"

TVRImporter::TVRImporter()
{
    //ctor
}

TVRImporter::~TVRImporter()
{
    //dtor
}

OBJCollection* TVRImporter::import(const char* f_path, Checker* check){
    ifstream fin;
    fin.open(f_path);

    if (!fin) return NULL;//error

    string inputstr;
    getline(fin, inputstr);
    if (inputstr.find("TVR0") == string::npos){
        cout << "different version : " <<  inputstr << endl;
        return NULL;
    }

    string group_name;
    int v_count = 0;
    int f_count = 0;

    Space* obj = NULL;
    OBJCollection* tvrcl = new OBJCollection();

    vector<Vertex*> sorted_vertex;
    while(!fin.eof()){
        getline(fin, inputstr);
        if (inputstr.size() < 3) continue;
        switch(inputstr.c_str()[0]){
            case 'c':{
                break;
            }
            case 'v':{
                Vertex vt;
                this->makeVertex(v_count, inputstr, vt);

                Vertex* pt_v = this->findSameVertex(sorted_vertex, check, vt);
                tvrcl->vertex.push_back(pt_v);

                v_count++;
                if (v_count % 5000 == 0) cout << "Loaded vertex : " << v_count << endl;

                break;
            }
            case 'g':{
                if (obj != NULL){
                    //obj->setVertexList(&tvrcl->vertex);
                    tvrcl->space_list.push_back(obj);
                }
                group_name = this->getGroupName(inputstr);
                if (group_name.find('\r') != string::npos) group_name.erase(group_name.find('\r'));

                obj = new Space(group_name, check);
                break;
            }
            case 'f':{
                f_count++;
                if (f_count % 5000 == 0) cout << "Loaded faces : " << f_count << endl;

                Triangle tri;
                this->makeTriangle(inputstr, tvrcl->vertex, tri);
                obj->pushTriangle(tri);

                obj->whole_area += tri.getArea();
                break;
            }
        }
    }
    obj->vertex = &tvrcl->vertex;
    tvrcl->space_list.push_back(obj);

    return tvrcl;
}


Vertex* TVRImporter::findSameVertex(vector<Vertex*>& vertex, Checker* check, Vertex& vt){
    std::vector<Vertex*>::iterator low, up, it;
    low = std::lower_bound(vertex.begin(), vertex.end(), vt.x(), CompareVertexAndX() );
    up = std::upper_bound(vertex.begin(), vertex.end(), vt.x(), CompareVertexAndX() );

    for (it = low ; it != up ; it++){
        if (check->compare_vertex( (*it), &vt) > 0){
            break;
        }
        else if (check->compare_vertex( (*it), &vt) < 0){
            continue;
        }
        else{
            cout << "Same Vertex" << endl;
            return *it;
        }
    }

    Vertex* new_vt = new Vertex(vt);
    vertex.insert(it, new_vt);

    return new_vt;
}

string TVRImporter::getGroupName(string& input){
    std::vector<std::string> x = split(input, ' ');
    string x_1(x[1]);
    x.clear();
    return x_1;
}

void TVRImporter::makeTriangle(string& input, vector<Vertex*>& vertex, Triangle& tri){
    std::vector<std::string> x = split(input, ' ');

    ll a = stol(x[1]);
    ll b = stol(x[2]);
    ll c = stol(x[3]);

    tri.a = vertex[a];
    tri.b = vertex[b];
    tri.c = vertex[c];


    x.clear();
}


void TVRImporter::makeVertex(int id, string& input, Vertex& vt){
    std::stringstream ss;
    ss.str(input);

    string line;
    getline(ss, line, '\t');
    std::vector<std::string> strings = split(line, ' ');

    //vt.x = stod(x[1]);
    vt.index = id;
    vt.setX(atof(strings[1].c_str()));
    vt.setY(stod(strings[2]));
    vt.setZ(stod(strings[3]));
    strings.clear();
}

int TVRImporter::extractMINtvr(string filename){
    ifstream fin;
    fin.open(filename+".tvr");

    ofstream fout;
    fout.open(filename+".min.tvr");
    if (!fin) return -1;//error

    string inputstr;
    getline(fin, inputstr);
    if (inputstr.find("TVR0") == string::npos){
        cout << "different version : " <<  inputstr << endl;
        return -1;
    }
    else{
        fout << "TVR0" << endl;
    }

    while(!fin.eof()){
        getline(fin, inputstr);
        if (inputstr.size() < 3) continue;
        switch(inputstr.c_str()[0]){
            case 'c':{
                break;
            }
            case 'v':{
                std::stringstream ss;
                ss.str(inputstr);
                string line;
                getline(ss, line, '\t');
                fout << line << endl;
                break;
            }
            case 'g':{
                fout << inputstr << endl;
                break;
            }
            case 'f':{
                fout << inputstr << endl;
                break;
            }
        }
    }
    fout.close();
    fin.close();

    return 0;
}


