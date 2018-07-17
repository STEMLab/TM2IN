//
// Created by dongmin on 18. 7. 17.
//

#ifndef TM2IN_OPTIONS_H
#define TM2IN_OPTIONS_H

#include <cstring>
#include <string>

using namespace std;

class Options {
public:
    Options(int argc, char **argv);

    string input_dir;
    string output_dir;

    string input_file; // "apt3.tvr"
    string output_file; // "ap3.json"
    string version; // "x.y.z"

    double threshold_1 = 1.0;
    double threshold_2 = 10.0;

    int input_type = 0;
    int polygonizer_mode = 0;

    bool output_3ds = false;
    bool output_tvr = false;
private:
    bool has_input_dir = false;
    bool has_output_dir = false;
    bool has_version = false;
    bool has_input_type = false;
    bool has_polygonizer = false;
    bool has_no_merge = false;

    void check_options();
};


#endif //TM2IN_OPTIONS_H
