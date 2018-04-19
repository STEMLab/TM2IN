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

void debug(){
    std::cout << "Only for Debuggin...remove it later" << std::endl;
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


/**
 * @param a
 * @param b lower case. the answer what I expect.
 * @return
 */
bool checkAnswer(char a, char b){
    if (a == b || a == b + 32){
        return true;
    }
    return false;
}