//
// Created by dongmin on 18. 7. 17.
//

#include "Options.h"
#include <getopt.h>
#include <iostream>
#include <cassert>


Options* Options::instance = NULL;

void Options::make_file_name(){
    if (!has_input_type){
        string file_extension = input_file.substr(input_file.find_last_of(".") + 1);
        if (file_extension == "tvr" || file_extension == "TVR"){
            input_type = 1;
        }
        else if (file_extension == "3ds" || file_extension == "3DS"){
            input_type = 2;
        }
        else if (file_extension == "dae" || file_extension == "DAE"){
            input_type = 3;
        }
    }

    std::size_t pos = input_file.find_last_of(".");
    file_name = input_file.substr(0, pos);
    fprintf(stdout, "File name : %s\n",file_name.c_str());

    output_dir += file_name;
    output_dir += "/";
    output_dir += version;
    output_dir += "/";
}

void Options::check_options() {
    if (!has_input_dir){
        throw std::runtime_error("--input-dir is missing..;\n");
    }

    if (!has_output_dir){
        throw std::runtime_error("--output-dir missing : where can I store..\n");
    }

    if (!has_input_type){
        fprintf(stderr, "WARNING : we will detect your file type and covert. Maybe it will be right..\n");
        fprintf(stderr, "IF you can make it sure, define value of --input-type option.\n\n");
    }
}

void Options::make(int argc, char **argv) {
    const char *short_opts = "i:O:v:r:tp:a:b:TDLI:GA:VC";
    const option long_opts[] = {
            {"input-dir",   1, 0, 'i'},
            {"output-dir",  1, 0, 'O'},
            {"version",     1, 0, 'v'},
            {"input-type",  1, 0, 'r'},
            {"no-merge",    0, 0, 't'},
            {"polygonizer", 1, 0, 'p'},
            {"thres1"     , 1, 0, 'a'},
            {"thres2",      1, 0, 'b'},
            {"output-tvr",  0, 0, 'T'},
            {"output-3ds",  0, 0, 'D'},
            {"output-tri",  0, 0, 'L'},
            {"indoorGML" , 1, 0, 'I'},
            {"write-process",0,0, 'G'},
            {"select-arch", 1, 0, 'A'},
            {"do-validation", 0, 0, 'V'},
            {"do-classification", 0, 0, 'C'},
            {0, 0, 0, 0}
    };
    int index = 0;
    int c;
    while (-1 != (c = getopt_long(argc, argv, short_opts, long_opts, &index))){
        string type_name;
        switch (c){
            case 'i':
                assert(optarg != NULL);
                input_dir = optarg;
                if (input_dir.back() != '/'){
                    input_dir.append("/");
                }
                has_input_dir = true;
                break;
            case 'O':
                assert(optarg != NULL);
                output_dir = optarg;
                if (output_dir.back() != '/'){
                    output_dir.append("/");
                }
                has_output_dir = true;
                break;
            case 'r':
                assert(optarg != NULL);
                type_name = optarg;
                has_input_type = true;
                if (type_name == "tvr" || type_name == "TVR"){
                    input_type = 1;
                }
                else if (type_name == "3ds" || type_name == "3DS"){
                    input_type = 2;
                }
                else if (type_name == "dae" || type_name == "DAE" || type_name == "collada" || type_name == "COLLADA"){
                    input_type = 3;
                }
                else{
                    printf("\n");
                    printf("Import mode should be one type of {tvr, 3ds, collada}");
                    printf("\n");
                }
                break;
            case 'v':
                version = optarg;
                break;
            case 't':
                has_no_merge = true;
                break;
            case 'p':
                has_polygonizer = true;
                polygonizer_mode = stoi(optarg);
                break;
            case 'a':
                assert(optarg != NULL);
                threshold_1 = strtod(optarg, NULL);
                break;
            case 'b':
                assert(optarg != NULL);
                threshold_2 = strtod(optarg, NULL);
                break;
            case 'T':
                break;
            case 'D':
                break;
            case 'G':
                generator = true;
                break;
            case 'A':
                assert(optarg != NULL);
                selected = stoi(optarg);
                break;
            case 'L':
                break;
            case 'V':
                do_validation = 1;
                break;
            case 'I':
                output_indoor_gml = true;
                infactory_url = optarg;
                break;
            case 'C':
                do_classification = true;
                break;
        }
    }

    if (optind < argc)
    {
        input_file = argv[optind++];
    }
    else{
        fprintf(stderr, "There is no input file name..\n");
        exit(EXIT_FAILURE);
    }

    if (optind < argc){
        fprintf(stderr, "Only one file is needed\n");
    }

    check_options();
    make_file_name();
}


