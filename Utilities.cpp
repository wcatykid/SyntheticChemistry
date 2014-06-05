#include <string>
#include <cstdio>
#include <cstring>

#include "Utilities.h"

//
// log base 2 of the input value.
//
double log2(double value)
{
    if (value <= 0) return 0;

    return log10(value) / log10(2.0);
}

//
// How many bits in binary to represent this number?
//
unsigned int numBinaryBits(unsigned int value)
{
    return floor(log2(value)) + 1;
}

std::string MakeString(const char s[], int val)
{
    char buff[strlen(s) + 32];

    sprintf(buff, "%s: %d", s, val);
    std::string temp = buff;

    return temp;
}

std::string MakeString(const char s1[], const char s2[])
{
    char buff[strlen(s1) + strlen(s2)];

    sprintf(buff, "%s: %s", s1, s2);
    std::string temp = buff;

    return temp;
}

std::string MakeString(const char s1[], std::string s2)
{
    char buff[strlen(s1) + s2.size()];

    sprintf(buff, "%s: %s", s1, s2.c_str());
    std::string temp = buff;

    return temp;
}