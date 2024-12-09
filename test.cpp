#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include<cmath>
#include<vector>

#define CURL_STATICLIB
#include <curl/curl.h>


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



//字符串切割
void split_string(std::string str, std::string delimiter, std::vector<std::string> &result)
{
    result.clear();
    int start = 0, end;
    int size = str.length();    //字符串长度
    while(start < size){
        if(str[start] == ' '){    //跳过空格
            start++;
            continue;
        }
        end = str.find_first_of(delimiter, start);   //子串结尾
        if(end < 0 || end >= size){      //找到字符串结尾，说明只剩最后一个子串
            result.push_back(str.substr(start, size - start));   //加入子串
            break;
        }
        if(end != start){    //找到子串
            result.push_back(str.substr(start, end - start));   //加入子串
            start= end + 1;     //继续向后切割
        }
        else{
            start ++;
        }
    }
}



//切割输入的选择器表达式
void split_selector(std::string selector, std::vector<std::string> &selectors)
{
    int start = 0, end;
    int size = selector.length();
    std::string delimiters1 = ".#*,>+~[";
    std::string delimiters2 = ".#*,>+~] ";
    if(delimiters1.find(selector[0], 0) == -1){    //如果表示式第一个为标签，加上一个空格
        selector = " " + selector;
        size++;
    }
    while(start < size){
        if(selector[start] == ' '){
            start ++;
            continue;
        } 
        end = selector.find_first_of(delimiters2, start + 1); 
        if(end == -1){
            end = size;
        }
        if((selector[start] >= 'A' && selector[start] <= 'Z')
            || selector[start] >= 'a' && selector[start] <= 'z'){
            selectors.push_back(" ");
            selectors.push_back(selector.substr(start, end - start));
        }
        else if(delimiters1.find(selector[start], 0) != -1){
            selectors.push_back(selector.substr(start, 1));
            selectors.push_back(selector.substr(start + 1, end - start - 1));
        }
        else {
            std::cout << "错误" << std::endl;
            return ;
        }
        start = end;
    }
}

int main()
{
    std::string select = ".class.class  href div+ div > div~a [div~=class1] div";
    std::vector<std::string> selectors;
    split_selector(select, selectors);

    return 0;
}