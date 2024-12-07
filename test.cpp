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


// 回调函数：处理网页内容并将其存储到 html 字符串中
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* html = reinterpret_cast<std::string*>(userp);
    html->append(reinterpret_cast<char*>(contents), total_size);
    return total_size;
}

// 获取网页内容的函数
bool fetch_url(std::string &html, const std::string& url) {
    CURL* curl = nullptr;
    CURLcode res;
    
    // 初始化 libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        // 设置要访问的 URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        // 设置回调函数来获取网页内容
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
        
        // 执行请求
        res = curl_easy_perform(curl);
        
        // 检查请求是否成功
        if (res != CURLE_OK) {
            html.clear();  // 如果请求失败，返回空字符串
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }
        
        // 清理 curl
        curl_easy_cleanup(curl);
    }
    
    // 清理 libcurl
    curl_global_cleanup();
    //预处理换行符
    int size = html.length();
    for(int i = 0; i < size; i++){
        if(html[i] == '\n'){
            html[i] = ' ';
        }
    }
    return true;  // 读取成功
}

int main()
{
    std::string html;
    std::string url = "https://grs.ruc.edu.cn/";
    if(fetch_url(html, url)){
        std::cout << html << std::endl;
    }
    return 0;
}