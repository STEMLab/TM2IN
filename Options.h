//
// Created by dongmin on 18. 7. 17.
//

#ifndef TM2IN_OPTIONS_H
#define TM2IN_OPTIONS_H

#include <cstring>
#include <string>
#include "config.h"

using namespace std;

enum ARCHITECTURE {ARCH, NON_ARCH, ALL, MANUAL};

class Options {
private:
    Options(){}

    static Options* instance;
public:
    static Options* getInstance(){
        if(!instance){
            instance = new Options();
        }
        return instance;
    }

    void make(int argc, char **argv);

    string input_dir;
    string output_dir;

    string input_file; // "apt3.tvr"
    string output_file; // "ap3.json"

    string file_name;
    string version; // "x.y.z"

    double threshold_1 = 1.0;
    double threshold_2 = 10.0;

    int input_type = 0;
    int polygonizer_mode = 0; // 1 : PCA Polygonize, 2 : Triangle Polygonize, 3 : Divided Polygonize

    bool output_3ds = false;
    bool output_tvr = false;
    bool output_indoor_gml = false;

    bool generator = false;
    int generation = 0;

    int selected = ARCH;
    int do_validation = 0;
    bool has_no_merge = false;
private:
    bool has_input_dir = false;
    bool has_output_dir = false;
    bool has_version = false;
    bool has_input_type = false;
    bool has_polygonizer = false;

    void check_options();

    void make_file_name();
};


#endif //TM2IN_OPTIONS_H
