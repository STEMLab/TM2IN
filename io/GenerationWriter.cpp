#include "GenerationWriter.h"
#include "io/json.h"

#include <boost/filesystem.hpp>
#include <Options.h>
#include <detail/io/JsonWriter.h>
#include "features/RoomBoundary/TriangulatedSurfaceMesh.h"
#include "features/Wall/TriangulatedSurface.h"



namespace TM2IN {
    namespace io {
        GenerationWriter* GenerationWriter::instance = NULL;
        void GenerationWriter::start(Room* room){
            assert(this->mode == 0);
            this->room_name = room->name;
            this->directory_path = Options::getInstance()->output_dir + "process/" + room->name;
            if (boost::filesystem::exists(directory_path)) {
                boost::filesystem::remove_all(directory_path);
            }
            boost::filesystem::create_directories(directory_path);

            this->mode = 1;
        }

        void GenerationWriter::write(RoomBoundary::TriangulatedSurfaceMesh* tsm){
            string output_path = this->directory_path + "/" + std::to_string(Options::getInstance()->generation) + ".json";
            ofstream fout(output_path);
            fout << "{ \n";
            fout << " \"spaces\" : [ \n";
            string result;
            result = "{\n";
            result += " \"name\" : \"" + this->room_name + "\", \n";
            result += " \"Surfaces\" : [ \n";
            for (unsigned int id = 0; id < tsm->surfaces.size(); id++) {
                result += TM2IN::detail::io::surface_to_json(*(tsm->surfaces[id]));
                if (id != tsm->surfaces.size() - 1) {
                    result += ", \n";
                } else {
                    result += " \n";
                }
            }
            result += "] \n";
            result += "}";
            fout << result << endl;
            fout << "]" << endl;
            fout << "}" << endl;
            fout.close();
        }

        void GenerationWriter::write(vector<Wall::TriangulatedSurface*>& ts_list){
            string output_path = this->directory_path + "/" + std::to_string(Options::getInstance()->generation) +
                                 ".json";
            ofstream fout(output_path);
            fout << "{ \n";
            fout << " \"spaces\" : [ \n";
            string result;
            result = "{\n";
            result += " \"name\" : \"" + this->room_name + "\", \n";
            result += " \"Surfaces\" : [ \n";
            for (unsigned int id = 0; id < ts_list.size(); id++) {
                result += TM2IN::detail::io::surface_to_json(*(ts_list[id]));
                if (id != ts_list.size() - 1) {
                    result += ", \n";
                } else {
                    result += " \n";
                }
            }
            result += "] \n";
            result += "}";
            fout << result << endl;
            fout << "]" << endl;
            fout << "}" << endl;
            fout.close();
        }

        void GenerationWriter::close(){
            this->mode = 0;
        }
    }
}

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