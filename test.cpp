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

void menu()
{
    std::cout << "################################### MENU ########################################" << std::endl;
    std::cout << "##    --------------------请根据操作输入相应的命令或数字-------------------    ##" << std::endl; 
    std::cout << "##          ***********   0.Exit(退出)                      ***********        ##" << std::endl; 
    std::cout << "##          ***********   1.LoadHTML(加载HTML文件)          ***********        ##" << std::endl; 
    std::cout << "##          ***********   2.CheckHTML(检查HTML代码合法性)   ***********        ##" << std::endl; 
    std::cout << "##          ***********   3.OutHTML(输出XPATH路径下的代码)  ***********        ##" << std::endl;
    std::cout << "##          ***********   4.OutText(输出XPATH路径下的文本)  ***********        ##" << std::endl;
    std::cout << "##          ***********   5.URLParser(解析URL)              ***********        ##" << std::endl;
    std::cout << "#################################################################################" << std::endl;
}


int main()
{
    menu();
    return 0;
}