#include"sqlist.h"
#include<fstream>


/*
----------------------------------------------基本操作-----------------------------------------------------
*/
//数据比较函数
int datacompare(elemtype e1, elemtype e2)
{
     if (e1.tag.length() > e2.tag.length()){    //e1 > e2 返回1
        return 1;
    }
    else if(e1.tag.length() < e2.tag.length()){  //e1 < e2 返回-1
        return -1;
    }
    else{
        if (e1.tag > e2.tag){
            return 1;
        }
        else if (e1.tag < e2.tag){
            return -1;
        }
    }
    return 0;     //e1 = e2 返回0
}

//顺序表重申请内存,申请成功自动释放原内存空间
elemtype* list_realloc(elemtype* src, elemtype* end, size_t size)
{
    //顺序表不存在
    if(!src | !end){
        std::cerr << "Segement fault: 未定义的错误" << std::endl;
        return nullptr;
    }
    elemtype* newbase = new elemtype[size];
    if(!newbase){      //新表空间申请失败
        std::cerr << "CapacityExpand Error: 扩容失败" << std::endl;
        return nullptr;
    } 
    elemtype *cur = src, *new_cur = newbase;    //遍历链表
    while(cur <= end){     
        *new_cur++ = *cur++;    //复制内存
    }
    delete [] src;     //释放原表空间
    src = nullptr;

    return newbase;     //返回新表数据域
}

//顺序表重申请内存,将src指向的空间复制长度为len的内存复制到新空间，申请成功自动释放原内存空间
elemtype* list_realloc(elemtype* src, int len, size_t size)
{
    //顺序表不存在
    if(!src | len < 0){
        std::cerr << "Segement fault: 未定义的错误" << std::endl;
        return nullptr;
    }
    elemtype* newbase = new elemtype[size];
    if(!newbase){      //新顺序表空间申请失败
        std::cerr << "CapacityExpand Error: 扩容失败" << std::endl;
        return nullptr;
    } 
    elemtype *cur = src, *new_cur = newbase;    //遍历顺序表
    for(int i = 0; i < len; i++){
        newbase[i] = src[i];
    }
    delete [] src;     //释放原顺序表空间
    src = nullptr;

    return newbase;     //返回新顺序表数据域
}


//顺序表初始化
void init_list(sqlist &L)
{
    //顺序表已存在
    if(L.elem){
        std::cerr << "Exist Error: 顺序表已存在" << std::endl;
        return ;
    }
    L.elem = new elemtype[LIST_INIT_SIZE];
    if(!L.elem){     //判断指针是否有效
        std::cerr << "Init Error: 顺序表初始化失败" << std::endl;
        return ;
    } 
    //申请成功，初始化顺序表
    L.length = 0;   //表长初始为0
    L.listsize = LIST_INIT_SIZE;    //容量设置为初始容量4096
}

//指定大小初始化
void init_list(sqlist &L, size_t size)
{
    //顺序表已存在
    if(L.elem){
        std::cerr << "Exist Error: 顺序表已存在" << std::endl;
        return ;
    }
    L.elem = new elemtype[size];
    if(!L.elem){     //判断指针是否有效
        std::cerr << "Init Error: 顺序表初始化失败" << std::endl;
        return ;
    } 
    //申请成功，初始化顺序表
    L.length = 0;   //表长初始为0
    L.listsize = size;    //容量设置为初始容量size
}

//顺序表销毁
void destroy_list(sqlist &L)
{
    if (!L.elem){     //顺序表不存在
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    delete[] L.elem;   //释放顺序表
    L.elem = nullptr;    //指针置空
    L.length = 0;       //长度和容量为0
    L.listsize = 0;
    return ;
}

//清空顺序表
void clear_list(sqlist &L)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    for(int i = 0; i < L.length; i++){   //清空内容
        L.elem[i].tag = "";
        L.elem[i].type = None;
    }
    L.length = 0;    //长度置为0

}

//判断顺序表是否为空
bool list_empty(sqlist L)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return false;
    }
    if (L.length > 0)     //元素个数大于0.返回真
    {
        return true;
    }
    return false;
}

//获取顺序表长度
int list_length(sqlist L)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return -1;
    }
    return L.length;
}

//获取第i个元素(i从1开始)
void get_elem(sqlist L, int i, elemtype &e)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    if (i < 1 || i > L.length){   //无效查询
        std::cerr << "Segement fault: 越界" << std::endl;
        return ;
    }
    e = L.elem[i - 1];
}

//查找元素e的位置
int locate_elem(sqlist L, elemtype e)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return -1;
    }
    for (int i = 0; i < L.length; i++){
        if (!(datacompare)(L.elem[i], e)){   //相等返回位置
            return i + 1;
        }
    }
    return -1;    //-1表示元素不存在
}

//查找元素e的直接前驱
void prior_elem(sqlist L, elemtype cur_e, elemtype &pre_e)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    int pos = locate_elem(L, cur_e);    //当前元素位置
    if (pos != -1 && pos > 1)     //检查是否越界，是否有前驱
    {
        pre_e = L.elem[pos - 2];   //返回前驱数据
        return ;
    }
    std::cerr << "Segement fault: 前驱不存在" << std::endl;
}

//查找元素e直接后驱
void next_elem(sqlist L, elemtype cur_e, elemtype &next_e)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    int pos = locate_elem(L, cur_e);     //当前元素位置
    if (pos != -1 && pos < L.length)    //检查是否越界，是否有后继
    {
        next_e = L.elem[pos];           //返回后继数据
        return ;
    }
    std::cerr << "Segement fault: 后继不存在" << std::endl;
}

//插入节点
void list_insert(sqlist &L, int i, elemtype e)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }  
    if (i < 1 || i > L.length + 1){       //判断是否越界
        std::cerr << "Segement fault: 越界" << std::endl;
        return ;
    }
    // if(!capacity_expansion(L)){   //判断并扩容
    //     std::cerr << "Expansion Error: 扩容失败" << std::endl;
    //     return ;
    // }
    if(L.length >= L.listsize){    //判断并扩容
        elemtype* newelem =  list_realloc(L.elem, L.length, L.listsize * 2);
        if(newelem){     //申请成功，更新链表
            L.elem = newelem;
            L.listsize = L.listsize * 2;
        }
        else{
            std::cerr << "Expand Error: 扩容失败" << std::endl;
            return ;
        }
    }
    if(i == L.length + 1){    //如果插入元素为表尾,无需移动元素
        L.elem[L.length] = e;
        L.length ++;
        return ;
    }
    //无需扩容或扩容成功，插入元素
    elemtype* q = L.elem + i - 1;
    for (elemtype* p = L.elem + L.length - 1; p >= q; p--){   //后移节点L.elem[i-1, ..., L.length - 1]
        *(p + 1) = *p; 
    }
    *q = e;
    L.length ++;   
}


//删除第i个节点
void list_delete(sqlist &L, int i, elemtype &e)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    if (i < 1 || i > L.length){   //判断是否越界
        return ;
    }
    elemtype* q = L.elem + L.length - 1;
    elemtype* p = L.elem + i - 1;
    e = *p;
    for (++p; p <= q; p++){   //前移节点L.elem[i - 1, L.length - 1]
        *(p - 1) = *p; 
    }
    L.length --;    //长度-1
}

//打印顺序表
void list_print(sqlist L)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    int end = L.length - 1;
    for (int i = 0; i < end; i++){    //打印前length-1个
        std::cout << L.elem[i].tag << std::endl;    //空格分隔
    }
    std::cout << L.elem[L.length - 1].tag << std::endl;    //打印最后一个，末尾不加空格
}

/*
--------------------------------------------初级功能-----------------------------------------------------
*/

//返回从start开始第一个分隔符的位置
size_t find_first_delimiter(std::string str, size_t size, size_t start, int &language)
{
    size_t len = size - 1;
    std::string english_signs = " ),./;:'[]`\"<>?!(|";
    std::string chinese_signs = "），’。（‘？“、”；》：【】·《！";

    if(start == len){            //如果start是str最后一个字节
        for(int k = 0; k < 18; k++){     
            if (str[len] == english_signs[k]){
                language = ENGLISH;
                return len;
            }
        }
    }
    for(int i = start; i < len; i++){
        for (int j = 0; j < 35; j+=2){     //判断中文标点符号
            if ((str[i] == chinese_signs[j]) && (str[i + 1] == chinese_signs[j + 1])){
                language = CHINESE;
                return i;
            }
        }
        for(int k = 0; k < 19; k++){     //判断英文标点符号和空格
            if (str[i] == english_signs[k]){
                language = ENGLISH;
                return i;
            }
        }
    }
    return size;
}


//返回从start开始第一个匹配子串的位置，找不到返回size,有空实现kmp算法
size_t find_first_substr(std::string str, std::string pattern, size_t start, size_t size)
{
    
    size_t plen = pattern.length();
    int i, j, k;
    for(i = start; i < size; i++){
        k = i;
        for (j = 0; j < plen; j++){     //判断中文标点符号
            if (str[k] != pattern[j]){
                break;
            }
            k++;
        }
        if(j == plen){
            return i;
        }
    }
    return size;
}

//返回从start开始第一个分隔符的位置，传入参数delimiters作为分隔符(只处理英文)
size_t find_first_delimiter(std::string str, size_t size, size_t start, std::string delimiters)
{
    size_t len = size - 1;
    size_t dlen = delimiters.length();
    for(int i = start; i < size; i++){
        for (int j = 0; j < dlen; j++){     //判断分隔符
            if (str[i] == delimiters[j]){
                return i;
            }
        }
    }
    return size;
}
