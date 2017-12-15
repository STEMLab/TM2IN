#include "logic/util.h"

#include <iostream>
#include <cmath>

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
