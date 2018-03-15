#include "TVRImporter.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include "features/Space.h"
#include "features/Vertex.h"

TVRImporter::TVRImporter()
{
    //ctor
}

TVRImporter::~TVRImporter()
{
    //dtor
}

TriangleMesh* TVRImporter::import(const char* f_path){
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

    TriangleMesh* tm = new TriangleMesh();
    vector<Triangle*> triangles;
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

                Vertex* pt_v = new Vertex(vt);
                // Vertex* pt_v = this->findSameVertex(sorted_vertex, check, vt);
                // pt_v->index = v_count;
                tm->vertices.push_back(pt_v);

                v_count++;
                if (v_count % 5000 == 0) cout << "Loaded vertices : " << v_count << endl;

                break;
            }
            case 'g':{
                if (f_count != 0){
                    tm->triangles.push_back(make_pair(group_name, triangles));
                    triangles.clear();
                    f_count = 0;
                }
                group_name = this->getGroupName(inputstr);
                if (group_name.find('\r') != string::npos) group_name.erase(group_name.find('\r'));

                break;
            }
            case 'f':{
                assert(tm->vertices.size() > 0);

                f_count++;
                if (f_count % 5000 == 0) cout << "Loaded faces : " << f_count << endl;

                Triangle* tri = this->makeTriangle(inputstr, tm->vertices);
                triangles.push_back(tri);

                break;
            }
        }
    }
    tm->triangles.push_back(make_pair(group_name, triangles));
    return tm;
}

/*
Vertex* TVRImporter::findSameVertex(vector<Vertex*>& vertices, Checker* check, Vertex& vt){
    vector<Vertex*>::iterator it, low;
    low = std::lower_bound(vertices.begin(), vertices.end(), vt.x() - check->threshold_vertex * 2, CompareVertexAndX() );

    for (it = low ; it != vertices.end() ; it++){
        double diff = check->compare_vertex( (*it), &vt);
        if (diff > 0){
            break;
        }
        else if (diff < 0){
            continue;
        }
        else{
            return *it;
        }
    }

    Vertex* new_vt = new Vertex(vt);
    vertices.insert(it, new_vt);

    return new_vt;
}
*/

string TVRImporter::getGroupName(string& input){
    std::vector<std::string> x = split(input, ' ');
    string x_1(x[1]);
    x.clear();
    return x_1;
}

Triangle* TVRImporter::makeTriangle(string& input, vector<Vertex*>& vertex){
    std::vector<std::string> x = split(input, ' ');

    ll a = stol(x[1]);
    ll b = stol(x[2]);
    ll c = stol(x[3]);

    Triangle* newTriangle = new Triangle(vertex[a], vertex[b], vertex[c]);

    x.clear();
    return newTriangle;
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


