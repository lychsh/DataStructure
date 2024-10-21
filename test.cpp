#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include<cmath>


void get_next(int next[], std::string pattern)
{
    int length = pattern.length();
    int j = 0, k = -1;
    next[1] = 0;
    while(j < length){
        if(k == -1 || pattern[k] == pattern[k]){
            k++;
            j++;
            next[j] = k;
        }
        else{
            k = next[k];
        }
    }
}


int kmp(std::string S, std::string T)
{
    int next[30] = {0};
    int slen = S.length(), tlen = T.length();
    int i = 0, j = -1;
    while(i < slen && j < tlen){
        if(S[i] == T[j] || j == -1){
            i++; j++;
        }
        else{
            j = next[j];
        }
    }
    if(j >= tlen){
        return i - tlen;
    }
    else{
        return -1;
    }
}


int main()
{
    std::string str1 = "abc";
    int length = str1.length();
    int next[length] = {0};
    for(int i = 0; i < length; i++){
        std::cout << next[i] << " " ;
    }
    return 0;
}