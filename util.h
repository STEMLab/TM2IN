#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <map>
#include <string>

typedef long long ll;
typedef unsigned long long ull;
typedef std::vector<std::pair<ll,ll>> vll;

std::vector<std::string> split(const std::string &s, char delim);

template<typename Out>
void split(const std::string &s, char delim, Out result);
void printProcess(ull index, ull size, std::string str);
void removeFilesInDirectory(std::string path);
void createAndRemoveDir();


#endif // UTIL_H_INCLUDED
