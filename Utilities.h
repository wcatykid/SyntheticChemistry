#ifndef _UTILITIES_GUARD
#define _UTILITIES_GUARD 1

#include <cmath>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

const unsigned int null = 0;

// Debugging constants
const bool DEBUG = true;
const bool HYPERGRAPH_CONSTR_DEBUG = true;


// log base 2 of the input value.
double log2(double value);

// How many bits in binary to represent this number?
unsigned int numBinaryBits(unsigned int value);

string MakeString(const char[], int);
std::string MakeString(const char s1[], const char s2[]);
std::string MakeString(const char s1[], std::string s2);

template<class T>
bool Contains(std::vector<T> list, const T& val)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].Equals(val)) return true;
    }
    return false;
}

#endif