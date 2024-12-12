#pragma once  
#include "stdafx.h"  
#include "UTN.h"  
  
std::wstring AsciiToUnicode(const std::string& str) {  
    // 预算-缓冲区中宽字节的长度    
    int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);  
    // 给指向缓冲区的指针变量分配内存    
    wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);  
    // 开始向缓冲区转换字节    
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);  
    std::wstring ret_str = pUnicode;  
    free(pUnicode);  
    return ret_str;  
}  
std::string UnicodeToAscii(const std::wstring& wstr) {  
    // 预算-缓冲区中多字节的长度    
    int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);  
    // 给指向缓冲区的指针变量分配内存    
    char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);  
    // 开始向缓冲区转换字节    
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);  
    std::string ret_str = pAssii;  
    free(pAssii);  
    return ret_str;  
}  
std::wstring Utf8ToUnicode(const std::string& str) {  
    // 预算-缓冲区中宽字节的长度    
    int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);  
    // 给指向缓冲区的指针变量分配内存    
    wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);  
    // 开始向缓冲区转换字节    
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen);  
    std::wstring ret_str = pUnicode;  
    free(pUnicode);  
    return ret_str;  
}  

std::string Utf8ToAnsi(const char* su8){
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, su8, strlen(su8), NULL, 0);
	wchar_t* wszMultiByte = new wchar_t[wcsLen+1];
	::MultiByteToWideChar(CP_UTF8, NULL, su8, strlen(su8), wszMultiByte, wcsLen);
	
	int ansiLen = ::MultiByteToWideChar(CP_UTF8, NULL, wszMultiByte, wcsLen(wszMultiByte), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansiLen + 1];
	szAnsi[ansiLen] = '\0';
	::MultiByteToWideChar(CP_UTF8, NULL, wszMultiByte, wcsLen(wszMultiByte), szAnsi, ansiLen, NULL, NULL);
	std::string = szAnsi;
	delete[] szAnsi;
	delete[] wszMultiByte;
	
	return s;
}
std::string UnicodeToUtf8(const std::wstring& wstr) {  
    // 预算-缓冲区中多字节的长度    
    int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);  
    // 给指向缓冲区的指针变量分配内存    
    char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);  
    // 开始向缓冲区转换字节    
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);  
    std::string ret_str = pAssii;  
    free(pAssii);  
    return ret_str;  
}  
std::string AsciiToUtf8(const std::string& str) {  
    return UnicodeToUtf8(AsciiToUnicode(str));  
}  
std::string Utf8ToAscii(const std::string& str) {  
    return UnicodeToAscii(Utf8ToUnicode(str));  
}  
// ASCII与Unicode互转  
CStringW    AsciiToUnicode_CSTR(const CStringA& str) {  
    return AsciiToUnicode(LPCSTR(str)).c_str();  
}  
CStringA    UnicodeToAscii_CSTR(const CStringW& wstr) {  
    return UnicodeToAscii(LPCWSTR(wstr)).c_str();  
}  
// UTF8与Unicode互转  
CStringW    Utf8ToUnicode_CSTR(const CStringA& str) {  
    return Utf8ToUnicode(LPCSTR(str)).c_str();  
}  
CStringA    UnicodeToUtf8_CSTR(const CStringW& wstr) {  
    return UnicodeToUtf8(LPCWSTR(wstr)).c_str();  
}  
// ASCII与UTF8互转  
CStringA    AsciiToUtf8_CSTR(const CStringA& str) {  
    return UnicodeToUtf8_CSTR(AsciiToUnicode_CSTR(str));  
}  
CStringA    Utf8ToAscii_CSTR(const CStringA& str) {  
    return UnicodeToAscii_CSTR(Utf8ToUnicode_CSTR(str));  
}  
// string 与 Int 互转  
int StringToInt(const std::string& str) {  
    return atoi(str.c_str());  
}  
std::string IntToString(int i) {  
    char ch[1024];  
    memset(ch, 0, 1024);  
    sprintf_s(ch, sizeof(ch), "%d", i);  
    return ch;  
}  
std::string IntToString(char i) {  
    char ch[1024];  
    memset(ch, 0, 1024);  
    sprintf_s(ch, sizeof(ch), "%c", i);  
    return ch;  
}  
std::string IntToString(double i) {  
    char ch[1024];  
    memset(ch, 0, 1024);  
    sprintf_s(ch, sizeof(ch), "%f", i);  
    return ch;  
}  
