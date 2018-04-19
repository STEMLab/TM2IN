#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>
#include <sstream>
#include <vector>
#include <utility>

typedef long long ll;
typedef unsigned long long ull;
typedef std::vector<std::pair<ll,ll>> vll;

std::vector<std::string> split(const std::string &s, char delim);

template<typename Out>
void split(const std::string &s, char delim, Out result);
void debug();
void printProcess(ull index, ull size, std::string str);
void removeFilesInDirectory(std::string path);
bool checkAnswer(char a, char b);

#endif // UTIL_H_INCLUDED
