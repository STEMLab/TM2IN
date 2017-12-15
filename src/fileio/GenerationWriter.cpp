#include "GenerationWriter.h"
#include "fileio/JSONMaker.h"

void GenerationWriter::writeGenerationJSON(int gen, vector<Space*>& space_list){
    ofstream fout;
    string f_path = process_path + "g_" + to_string(gen) + ".json";
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return ;
    if (JSONMaker::printJSON(fout, space_list))
    {
        return ;
    }
    fout.close();
}

void GenerationWriter::clearGenerationStat(){
    ofstream statout;
    statout.open(process_path+ "gs" + ".txt", ios::out|ios::trunc);
    statout.close();
}

void GenerationWriter::writeGenerationStat(int gen, ull num){
    ofstream statout;
    statout.open(process_path+ "gs" + ".txt", ios::app);
    statout << "Generation " << gen << " : " << num << endl;
    statout.close();
}

