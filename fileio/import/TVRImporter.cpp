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

vector<TriangleMesh*> TVRImporter::import(const char* f_path){
    ifstream fin;
    fin.open(f_path);

    assert(fin);

    string inputstr;
    getline(fin, inputstr);
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

    while(!fin.eof()){
        getline(fin, inputstr);
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

/*
Vertex* TVRImporter::findSameVertex(vector<Vertex*>& vertices, Checker* check, Vertex& vt){
    vector<Vertex*>::iterator it, low;
    low = std::lower_bound(vertices.begin(), vertices.end(), vt.x() - check->thresholdVertex * 2, CompareVertex_X() );

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



Vertex* TVRImporter::makeVertex(int id, string &input){
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

int TVRImporter::extractMINtvr(string fileName){
    ifstream fin;
    fin.open(fileName+".tvr");

    ofstream fout;
    fout.open(fileName+".min.tvr");
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


