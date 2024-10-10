#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
#define CHINESE 2
#define ENGLISH 1


std::string::size_type find_first_delimiter(std::string str, std::string::size_type size, std::string::size_type start, int &language)
{
    std::string::size_type len = size - 1;
    std::string english_signs = " ,./;:'[]\\`\"<>?";
    std::string chinese_signs = "，。？、；：‘“’”【】·《》";

    if(start == len - 1){            //如果start是str最后一个字节
        for(int k = 0; k < 17; k++){     
            if (str[len] == english_signs[k]){
                language = ENGLISH;
                return len - 1;
            }
        }
    }
    for(int i = start; i < len; i++){
        for(int k = 0; k < 17; k++){     //判断英文标点符号和空格
            if (str[i] == english_signs[k]){
                language = ENGLISH;
                return i;
            }
        }
        for (int j = 0; j < 29; j+=2){     //判断中文标点符号
            if (str[i] == chinese_signs[j] && str[i + 1] == chinese_signs[j + 1]){
                language = CHINESE;
                return i;
            }
        }
    }
    return size;
    
}

unsigned float_i2f(int x) {
    unsigned int exp = 1, frac = 0, sign, round;
    if(!x){      //0返回0
        return 0;
    }
    if(!(x ^ 1 << 31)){   //Tmin特殊处理
        return 0xCF << 24;
    }
    sign = (1 << 31) & x;
    if(sign){    //负数转化为正数
        x = -x;
    }
    while(x >> exp){    //找到x的最高位的位置
        exp += 1;
    }
    frac = x << (32 - exp);    
    round = frac & 0xFF;
    frac = (frac << 1) >> 9;    //去掉整数部分1，得到小数
    if(round > 0x80 | (round == 0x80 & (frac & 1) )){   //需要进位的情况
        frac += 1;
    }
    if(frac >> 23){   //如果进位导致增加1位
        exp += 1;
        frac = (frac << 11) >> 11;
    }
    return sign | (exp + 0x7E) << 23 | frac;
}

unsigned float_i2f1(int x) {
    unsigned sign = x >> 31 & 1, exp, frac, round;
    int x_exp, frac_mask;
    if (!x) // x=0，会在求x最高非零位时出错，所以特判
        return 0;
    if (!(x ^ (1 << 31))) // x=TMin，会在下一步对x取反过程中出错，所以特判
        return 0xcf << 24;
    if (sign)
        x = -x;
    // x_exp 代表 x 的最高非零位的位置，也即 x 的精度位的最高位的位置
    x_exp = 31;
    while (!(x >> x_exp))
        x_exp--;
    exp = x_exp + 0x7f; // exp+bias
    x <<= (31 - x_exp); // 得到小数部分 
    frac_mask = 0x7fffff;
    // 右移 8 位，使得尾数位对齐
    frac = (x >> 8) & frac_mask;
    round = x & 0xff; // 得到要被舍入的小数部分
    frac += ((round > 0x80) || ((round == 0x80) && (frac & 1))); // 等价于判断是否大于128，或者等于128且最低位为1，即向偶数舍入
    // 对于舍入后进位的情况，因为最高位从23变为24，所以要且上一个掩码，而且增加一位阶码
    if (frac >> 23) {
        frac &= frac_mask;
        exp += 1;
    }
    return sign << 31 | (exp << 23) | frac;
}

int byteSwap(int x, int n, int m) {
    int clean, swap;
    n <<= 3;
    m <<= 3;
    clean = ~((0xFF << n) | (0xFF << m));
    swap = (x >> n & 0xFF) << m | (x >> m & 0xFF) << n;
	return (x & clean) | swap;
}

int logtwo(int v) {
    int r = 0;
    if(v >> 16) {r += 16; v = v >> 16;}
    if(v >> 8) {r += 8; v = v >> 8;}
    if(v >> 4) {r += 4; v = v >> 4;}
    if(v >> 2) {r += 2; v = v >> 2;}
    if(v >> 1) {r += 1;}
    return r;
}

unsigned floatScale2(unsigned uf) {
    unsigned exp, frac;
    exp = (uf << 1) >> 24;
    frac = uf << 9;
    if((exp > 0xFD) & frac){
        return uf;
    }
    if((!exp) & !frac){
        return uf;
    }
    exp += 1;
    return (uf & ~(0xFF << 23)) | (exp << 23);
}

unsigned reverse(unsigned v) {
    int r = 0;
    for(int i = 0; i < 32; i++){
        r |= (v >> i) & 1;
        r <<= 1;
    }
    return v;
}

int main()
{

    // std::string str;
    // std::cin >> str;
    // std::cout << str << std::endl;
    // std::string sign = "(";
    // if(str[0] == sign[0] && str[1] == sign[1]) {
    //     std::cout << "haha" << std::endl;
    // }
    std::string english_signs = " ),./;:'[]`\"<>?!(|";
    std::string chinese_signs = "），’。（‘？“、”；》：【】·《！";
    std::cout << chinese_signs.length() << std::endl;
    std::cout << english_signs.length() << std::endl;
    std::cout << english_signs[0] << std::endl;
    //std::cout << str[0] << str[1] << str[3] << std::endl;
    return 0;
}