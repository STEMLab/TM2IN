#include "ProcessWriter.h"
#include "fileio/JSONMaker.h"

void ProcessWriter::writeGenerationJSON(int gen, vector<Space*>& space_list){
    ofstream fout;
    string f_path = string(process_path) + data_name + "_g_" + to_string(gen) + ".json";
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return ;
    if (JSONMaker::printJSON(fout, space_list))
    {
        return ;
    }
    fout.close();
}

void ProcessWriter::writeGeneration(int gen, ull num, int space_num){
    ofstream statout;
    statout.open(string(process_path)+ data_name + "_" + to_string(space_num) + ".txt", ios::app);
    statout << "Generation " << gen << " : " << num << endl;
    statout.close();
}

void ProcessWriter::writeBeforeJoin(ull num, int space_num){
    ofstream statout;
    statout.open(string(process_path)+ data_name + "_" + to_string(space_num) + ".txt", ios::out|ios::trunc);
    statout << "Before Join : " << num << endl;
    statout.close();
}

void ProcessWriter::writeRoughSurfaces(ull num, int space_num){
    ofstream statout;
    statout.open(string(process_path)+ data_name + "_" + to_string(space_num) + ".txt", ios::app);
    statout << "Rough Surfaces : " << num << endl;
    statout.close();
}
