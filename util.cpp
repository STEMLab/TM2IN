#include "util.h"

#include <iostream>
#include <cmath>
#include <dirent.h>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

void printProcess(ull index, ull size, std::string str){
    std::cout << "\r" << str << " ==========" << (int)((double)index/(double)size * 100) <<"% ========";
}

void removeFilesInDirectory(std::string path){
    DIR *theFolder = opendir(path.c_str());
    struct dirent *next_file;
    char filepath[256];

    while ( (next_file = readdir(theFolder)) != NULL )
    {
        // build the path for each file in the folder
        sprintf(filepath, "%s/%s", path.c_str(), next_file->d_name);
        remove(filepath);
    }
    closedir(theFolder);
}

void createAndRemoveDir(const std::string &version, const std::string &resultPath) {
    if (boost::filesystem::exists(resultPath)){
        if (boost::filesystem::exists(resultPath + version)){
            char ans;
            std::cout << "\n\nThis version " << version << " folder exist. Remove Files in directory? (y/n)" << std::endl;
            std::cin >> ans;
            if (ans == 'y' || ans == 'Y')
                boost::filesystem::remove_all(resultPath +version);
        }
        else{
            boost::filesystem::create_directory(resultPath + version);
        }
    }
    else{
        boost::filesystem::create_directory(resultPath);
        boost::filesystem::create_directory(resultPath+ version);
    }
}