#include "GenerationWriter.h"
#include "io/json.h"

#include <boost/filesystem.hpp>

/*
void GenerationWriter::start(Room* p_space){
    this->space = p_space;

    //Create Generation directory
    if (boost::filesystem::exists(process_path + this->space->name)) {
        boost::filesystem::remove_all(process_path + this->space->name);
    }
    boost::filesystem::create_directories(process_path + this->space->name);

    ofstream statout;
    statout.open(process_path + this->space->name + "/gs" + ".txt", ios::out|ios::trunc);
    statout.close();

    this->write();
}

void GenerationWriter::write() {
    this->writeJSON();
    this->writeStat();
}

void GenerationWriter::writeJSON(){
    ofstream fout;
    string f_path = process_path + this->space->name + "/g_" + to_string(this->space->generation) + ".json";
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return ;
    fout << this->space->asJsonText();
    fout.close();
}


void GenerationWriter::writeStat(){
    ofstream statout;
    statout.open(process_path + this->space->name + "/gs" + ".txt", ios::app);
    statout << this->space->generation;
    statout << " , " << space->surfacesList.size();
    statout << "\n";
    statout.close();
}

*/