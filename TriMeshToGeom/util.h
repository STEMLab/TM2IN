#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>
#include <sstream>
#include <vector>

typedef long long ll;
typedef unsigned long long ull;

std::vector<std::string> split(const std::string &s, char delim);

template<typename Out>
void split(const std::string &s, char delim, Out result);

#endif // UTIL_H_INCLUDED
