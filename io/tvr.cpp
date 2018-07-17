//
// Created by dongmin on 18. 7. 16.
//

#include <detail/io/TVRReader.h>
#include "tvr.h"

namespace TM2IN {
    namespace io {
        vector<TriangleMesh *> importTVR(const char *filePath) {
            ifstream fin;
            fin.open(filePath);

            assert(fin);

            TM2IN::detail::io::TVRReader reader(fin);
            return reader.read();
        }
    }
}


/*
 *
 *
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



Vertex* TVRImporter::findSameVertex(vector<Vertex*>& vertices, Checker* check, Vertex& vt){
    vector<Vertex*>::iterator it, low;
    low = std::lower_bound(vertices.begin(), vertices.end(), vt.x() - check->threshold_same_vt_distance * 2, CompareVertex_X() );

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
