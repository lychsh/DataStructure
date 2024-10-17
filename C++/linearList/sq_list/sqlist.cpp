#include"sqlist.h"
#include<fstream>


/*
----------------------------------------------基本操作-----------------------------------------------------
*/
//数据比较函数
int datacompare(elemtype e1, elemtype e2)
{
     if (e1.length() > e2.length()){    //e1 > e2 返回1
        return 1;
    }
    else if(e1.length() < e2.length()){  //e1 < e2 返回-1
        return -1;
    }
    else{
        if (e1 > e2){
            return 1;
        }
        else if (e1 < e2){
            return -1;
        }
    }
    return 0;     //e1 = e2 返回0
}

//链表重申请内存,申请成功自动释放原内存空间
elemtype* list_realloc(elemtype* src, elemtype* end, size_t size)
{
    //顺序表不存在
    if(!src | !end){
        std::cerr << "Segement fault: 未定义的错误" << std::endl;
        return nullptr;
    }
    elemtype* newbase = new elemtype[size];
    if(!newbase){      //新链表空间申请失败
        std::cerr << "CapacityExpand Error: 扩容失败" << std::endl;
        return nullptr;
    } 
    elemtype *cur = src, *new_cur = newbase;    //遍历链表
    while(cur <= end){     
        *new_cur++ = *cur++;    //复制内存
    }
    delete [] src;     //释放原链表空间
    src = nullptr;

    return newbase;     //返回新链表数据域
}

//链表重申请内存,将src指向的空间复制长度为len的内存复制到新空间，申请成功自动释放原内存空间
elemtype* list_realloc(elemtype* src, int len, size_t size)
{
    //顺序表不存在
    if(!src | len < 0){
        std::cerr << "Segement fault: 未定义的错误" << std::endl;
        return nullptr;
    }
    elemtype* newbase = new elemtype[size];
    if(!newbase){      //新链表空间申请失败
        std::cerr << "CapacityExpand Error: 扩容失败" << std::endl;
        return nullptr;
    } 
    elemtype *cur = src, *new_cur = newbase;    //遍历链表
    for(int i = 0; i < len; i++){
        newbase[i] = src[i];
    }
    delete [] src;     //释放原链表空间
    src = nullptr;

    return newbase;     //返回新链表数据域
}


//表满扩容
bool capacity_expansion(sqlist &L)
{
    //顺序表不存在
    if(L.elem == nullptr){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return false;
    }
    if (L.length < L.listsize){    //未满无需扩容
        return true;
    }
    if (L.length >= MAX_INCREMENT){   //大于最大内存允许
        std::cerr << "Memory Overflow: 内存溢出" << std::endl;
        return false;
    }
    //判断容量，决定扩容方式

    if (L.listsize >= MAX_DOUBLE_INCREMENT){       //大于最大倍增容量，固定扩增
        if (L.listsize + FIXED_INCREMENT > MAX_INCREMENT){ //扩容后大于最大允许容量，设置为最大允许容量
            elemtype* newL = (elemtype *)list_realloc(L.elem, L.elem + L.length - 1, MAX_INCREMENT);
            if(newL != nullptr){       //申请内存成功,更新指针
                L.elem = newL;
                L.listsize = MAX_INCREMENT;
                return true;
            }
            else{
                std::cerr << "Expansion Error: 扩容失败" << std::endl;
                return false;
            }
        }
        else{           //扩容后小于最大允许容量，增加固定分配容量
            elemtype* newL = (elemtype *)list_realloc(L.elem, L.elem + L.length - 1, L.listsize + FIXED_INCREMENT);
            if(newL != nullptr){       //申请内存成功,更新指针
                L.elem = newL;
                L.listsize += FIXED_INCREMENT;
                return true;
            }
            else{
                std::cerr << "Expansion Error: 扩容失败" << std::endl;
                return false;
            }
        }
    }
    else if (L.listsize >= List_INIT_SIZE){     //达到初始分配量，倍增扩容
        elemtype* newL = (elemtype *)list_realloc(L.elem, L.elem + L.length - 1, L.listsize * MULTIPLIER_FACTOR);
            if(newL != nullptr){       //申请内存成功,更新指针
                L.elem = newL;
                L.listsize *= MULTIPLIER_FACTOR;
                return true;
            }
            else{
                std::cerr << "Expansion Error: 扩容失败" << std::endl;
                return false;
            }
    }
    return false;
}

//空间利用率过低，缩容
bool capacity_reduce(sqlist &L)
{
    //顺序表不存在
    if(L.elem == nullptr){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return false;
    }
    if (L.listsize == List_INIT_SIZE){
        return true;
    }
    //表长小于初始分配容量且表容量大于初始分配容量，表容量设置为初始分配容量
    if (L.length < List_INIT_SIZE && L.listsize > List_INIT_SIZE){    
        elemtype* newL = (elemtype *)list_realloc(L.elem, L.elem + L.length - 1, List_INIT_SIZE);
        if(newL != nullptr){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize = List_INIT_SIZE;
            return true;
        }
        else{
            std::cerr << "Reduce Error: 缩容失败" << std::endl;
            return false;
        }
    }
    // 表长小于表容量一半,表容量大于初始分配容量的2倍，小于最大倍增容量,表容量减半
    else if (L.length < L.listsize / 2 && L.listsize > List_INIT_SIZE * MULTIPLIER_FACTOR && L.listsize <= MAX_DOUBLE_INCREMENT){
        elemtype* newL = (elemtype *)list_realloc(L.elem, L.elem + L.length - 1, L.listsize / 2);
        if(newL != nullptr){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize /= 2;
            return true;
        }
        else{
            std::cerr << "Reduce Error: 缩容失败" << std::endl;
            return false;
        } 
    }
    //表容量大于最大倍增容量，表容量设置为最大倍增容量
    else if (L.listsize > MAX_DOUBLE_INCREMENT && L.length < MAX_DOUBLE_INCREMENT){
        elemtype* newL = (elemtype *)list_realloc(L.elem, L.elem + L.length - 1, MAX_DOUBLE_INCREMENT);
        if(newL != nullptr){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize = MAX_DOUBLE_INCREMENT;
            return true;
        }
        else{
            std::cerr << "Reduce Error: 缩容失败" << std::endl;
            return false;
        }    

    }  
    //表长大于最大倍增容量，表容量大于表长+固定分配容量，表容量减去固定分配容量
    else if(L.length < L.listsize - FIXED_INCREMENT && L.length > MAX_DOUBLE_INCREMENT && L.listsize < MAX_INCREMENT){
        elemtype* newL = (elemtype *)list_realloc(L.elem, L.elem + L.length - 1, L.listsize - FIXED_INCREMENT);
        if(newL != nullptr){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize -= FIXED_INCREMENT;
            return true;
        }
        else{
            std::cerr << "Reduce Error: 缩容失败" << std::endl;
            return false;
        }    
    }
    return false;
}

//顺序表初始化
void init_list(sqlist &L)
{
    //顺序表已存在
    if(L.elem){
        std::cerr << "Exist Error: 顺序表已存在" << std::endl;
        return ;
    }
    L.elem = new elemtype[List_INIT_SIZE];
    if(!L.elem){     //判断指针是否有效
        std::cerr << "Init Error: 顺序表初始化失败" << std::endl;
        return ;
    } 
    //申请成功，初始化顺序表
    L.length = 0;   //表长初始为0
    L.listsize = INIT_SIZE;    //容量设置为初始容量4096
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
        L.elem[i] = "";
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
        std::cout << L.elem[i] << " " ;    //空格分隔
    }
    std::cout << L.elem[L.length - 1] << std::endl;    //打印最后一个，末尾不加空格
}

/*
--------------------------------------------初级功能-----------------------------------------------------
*/

//返回从start开始第一个分隔符的位置
size_type find_first_delimiter(elemtype str, elemtype::size_type size, elemtype::size_type start, int &language)
{
    std::string::size_type len = size - 1;
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
size_type find_first_substr(std::string str, std::string pattern, size_type start, size_type size)
{
    std::string::size_type len = size - 1;
    std::string::size_type plen = pattern.length();
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
size_type find_first_delimiter(elemtype str, elemtype::size_type size, elemtype::size_type start, std::string delimiters)
{
    std::string::size_type len = size - 1;
    std::string::size_type dlen = delimiters.length();
    for(int i = start; i < size; i++){
        for (int j = 0; j < dlen; j++){     //判断分隔符
            if (str[i] == delimiters[j]){
                return i;
            }
        }
    }
    return size;
}


//从字符串str切词结果创造顺序表
void create_list(sqlist &L, std::string str)
{
    //顺序表不存在
    if(!L.elem){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word; 
    int language = 1;     //分隔符中文还是英文，决定下一次查找跳过一个字节还是两个
    int count = 0;   //切词个数
    if(size <= 0){      //字符串为空，置空顺序表
        clear_list(L);
        return ;
    }
    while(start <= (size - 1)){
        end = find_first_delimiter(str, size, start, language);
        if (end != start){
            word = str.substr(start, end - start);       //切词
            if(word == "" || word == " "){     //无效词跳过
                start = start + language; 
                continue;
            }
            count ++;     //词数+1
            list_insert(L, count, word);   //插入顺序表
        }
        start = end + language;
    }
}

//覆盖原字符串
void cover_list(sqlist &L, elemtype str)
{
    //原链表不存在或者存在但链表为空
    if(!L.elem || L.length == 0){
        std::cerr << "Exist Error: 顺序表不存在" << std::endl;
        return ;
    }
    int count = 1;   //切词个数
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word;    //切词结果
    int language = 0;
    int length = L.length;  //原表长

    if(size <= 0){      //字符串为空，置空链表
        clear_list(L);
        return ;
    }
    while(start <= size - 1 && count < L.listsize){       //字符串未切完并且链表有空位  
        end = find_first_delimiter(str, size, start, language);
        if (end != start){
            word = str.substr(start, end - start);       //切词
            if(word == "" || word == " "){     //无效词跳过
                start = start + language; 
                continue;
            }
            if(count >= L.listsize){    //链表已满，剩余词插入到链表末尾
                list_insert(L, count, word);   //插入顺序表
            }
            else{
                L.elem[count - 1] = word;
            }
            count ++;     //词数+1
        }
        start = end + language; 
    }
    L.length = count;
    //如果少于原来的顺序表长度，多余部分清空
    if(start >= size){
        for(int i = count ; i < length - 1; i++){
            L.elem[i] = "";
        }
    }
}

//从键盘输入链表内容(尾插)
void create_list_from_input(sqlist &L)
{
    //输入字符串
    std::string str;     
    if (std::getline(std::cin, str)) {  //读取成功
        if (L.elem != nullptr || L.length > 0){   // 顺序表存在且不为空，直接覆盖原链表
            cover_list(L, str);
            return ;
        }

        //顺序表不存在，创建顺序表
        if(L.elem == nullptr){
            init_list(L);    //初始化顺序表
            if (L.elem == nullptr){                      //判断顺序表是否创建成功
                std::cerr << "Input Error: 创建失败" << std::endl;
                return ; 
            }    
        }
        create_list(L, str);   
    }
    // 读取失败
    else{
        std::cerr << "KeyboardInput error ：输入失败: " << std::endl;
        return ;
    }
}

//从文件读入链表内容(尾插)
void create_list_from_file(sqlist &L, std::string path)
{
    std::ifstream file(path); // 打开文件用于读取
    if (!file.is_open()) {    // 如果文件不能打开
        std::cerr << "FileOpen error: 文件读取失败" << std::endl;
        return ;
    }
    //输入字符串
    elemtype str;     
    if (std::getline(file, str)) {  //读取成功
        if (L.elem != nullptr || L.length > 0){   // 顺序表存在且不为空，直接覆盖原链表
            cover_list(L, str);
            return ;
        }

        //顺序表不存在，创建顺序表
        if(L.elem == nullptr){
            init_list(L);    //初始化顺序表
            if (L.elem == nullptr){                      //判断顺序表是否创建成功
                std::cerr << "Input Error: 创建失败" << std::endl;
                return ; 
            }    
        }
        create_list(L, str);   
    }
    file.close();
}