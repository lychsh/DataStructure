#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include <sstream>
#include <string>


void pattern_next(int next[100], std::string pattern)
{
    int end = pattern.length();
    int left = 0, right, val;
  
}


int main()
{
    std::string pattern  = "abaabcac";
    int next[100] = {0, 1};
    pattern_next(next, pattern);
    for (int i = 0; i < pattern.length(); i++){
        std::cout << next[i] << " " << std::endl;
    }
    return 0;
}