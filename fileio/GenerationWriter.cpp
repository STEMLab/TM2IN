#include "GenerationWriter.h"
#include "fileio/JSONMaker.h"

#include <boost/filesystem.hpp>

void GenerationWriter::start(Space* p_space){
    this->space = p_space;

    //Create Generation directory
    if (boost::filesystem::exists(process_path + this->space->name)) {
        boost::filesystem::remove_all(process_path + this->space->name);
    }
    boost::filesystem::create_directories(process_path + this->space->name);

    ofstream statout;
    statout.open(process_path + this->space->name + "/gs" + ".txt", ios::out|ios::trunc);
    statout.close();

    generation = 0;
}

void GenerationWriter::write() {
    this->writeJSON();
    this->writeStat();
    generation++;
}

void GenerationWriter::writeJSON(){
    ofstream fout;
    string f_path = process_path + this->space->name + "/g_" + to_string(generation) + ".json";
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return ;
    if (JSONMaker::printJSON(fout, space))
    {
        return ;
    }
    fout.close();
}


void GenerationWriter::writeStat(){
    ofstream statout;
    statout.open(process_path + this->space->name + "/gs" + ".txt", ios::app);
    statout << "Generation " << generation << " : " << space->surfacesList.size() << endl;
    statout.close();
}

