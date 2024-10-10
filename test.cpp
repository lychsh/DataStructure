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
    std::string chinese_signs = "������������������������������";

    if(start == len - 1){            //���start��str���һ���ֽ�
        for(int k = 0; k < 17; k++){     
            if (str[len] == english_signs[k]){
                language = ENGLISH;
                return len - 1;
            }
        }
    }
    for(int i = start; i < len; i++){
        for(int k = 0; k < 17; k++){     //�ж�Ӣ�ı����źͿո�
            if (str[i] == english_signs[k]){
                language = ENGLISH;
                return i;
            }
        }
        for (int j = 0; j < 29; j+=2){     //�ж����ı�����
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
    if(!x){      //0����0
        return 0;
    }
    if(!(x ^ 1 << 31)){   //Tmin���⴦��
        return 0xCF << 24;
    }
    sign = (1 << 31) & x;
    if(sign){    //����ת��Ϊ����
        x = -x;
    }
    while(x >> exp){    //�ҵ�x�����λ��λ��
        exp += 1;
    }
    frac = x << (32 - exp);    
    round = frac & 0xFF;
    frac = (frac << 1) >> 9;    //ȥ����������1���õ�С��
    if(round > 0x80 | (round == 0x80 & (frac & 1) )){   //��Ҫ��λ�����
        frac += 1;
    }
    if(frac >> 23){   //�����λ��������1λ
        exp += 1;
        frac = (frac << 11) >> 11;
    }
    return sign | (exp + 0x7E) << 23 | frac;
}

unsigned float_i2f1(int x) {
    unsigned sign = x >> 31 & 1, exp, frac, round;
    int x_exp, frac_mask;
    if (!x) // x=0��������x��߷���λʱ������������
        return 0;
    if (!(x ^ (1 << 31))) // x=TMin��������һ����xȡ�������г�����������
        return 0xcf << 24;
    if (sign)
        x = -x;
    // x_exp ���� x ����߷���λ��λ�ã�Ҳ�� x �ľ���λ�����λ��λ��
    x_exp = 31;
    while (!(x >> x_exp))
        x_exp--;
    exp = x_exp + 0x7f; // exp+bias
    x <<= (31 - x_exp); // �õ�С������ 
    frac_mask = 0x7fffff;
    // ���� 8 λ��ʹ��β��λ����
    frac = (x >> 8) & frac_mask;
    round = x & 0xff; // �õ�Ҫ�������С������
    frac += ((round > 0x80) || ((round == 0x80) && (frac & 1))); // �ȼ����ж��Ƿ����128�����ߵ���128�����λΪ1������ż������
    // ����������λ���������Ϊ���λ��23��Ϊ24������Ҫ����һ�����룬��������һλ����
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
    std::string chinese_signs = "������������������������������������";
    std::cout << chinese_signs.length() << std::endl;
    std::cout << english_signs.length() << std::endl;
    std::cout << english_signs[0] << std::endl;
    //std::cout << str[0] << str[1] << str[3] << std::endl;
    return 0;
}