#include "TVRImporter.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include "features/Solid.h"
#include "features/Vertex.h"


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


