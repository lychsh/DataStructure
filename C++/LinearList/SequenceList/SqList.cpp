#include"SqList.h"

//数据比较函数
bool strcompare(ElemType e1, ElemType e2)
{
    return strcmp(e1.str, e2.str) == 0;
}

Compare compare = strcompare;
//表满扩容
void CapacityExpansion(SqList &L)
{
    if (L.length < L.listsize){    //未满无需扩容
        return ;
    }
    if (L.length >= MAX_INCREMENT){   //大于最大内存允许
        cout << "----超过最大允许内存----" << endl;
        return ;
    }
    //判断容量，决定扩容方式

    if (L.listsize >= MAX_DOUBLE_INCREMENT){       //大于最大倍增容量，固定扩增
        if (L.listsize + FIXED_INCREMENT > MAX_INCREMENT){ //扩容后大于最大允许容量，设置为最大允许容量
            ElemType* newL = (ElemType *)realloc(L.elem, MAX_INCREMENT * sizeof(ElemType));
            if(newL != NULL){       //申请内存成功,更新指针
                L.elem = newL;
                L.listsize = MAX_INCREMENT;
            }
            else{
                cout << "----扩容失败----" << endl;
                return ;
            }
        }
        else{           //扩容后小于最大允许容量，增加固定分配容量
            
            ElemType* newL = (ElemType *)realloc(L.elem, (L.listsize + FIXED_INCREMENT) * sizeof(ElemType));
            if(newL != NULL){       //申请内存成功,更新指针
                L.elem = newL;
                L.listsize += FIXED_INCREMENT;
            }
            else{
                cout << "----扩容失败----" << endl;
                return ;
            }
        }
    }
    else if (L.listsize >= LIST_INIT_SIZE){     //达到初始分配量，倍增扩容
        ElemType* newL = (ElemType *)realloc(L.elem, (L.listsize * MULTIPLIER_FACTOR) * sizeof(ElemType));
            if(newL != NULL){       //申请内存成功,更新指针
                L.elem = newL;
                L.listsize *= MULTIPLIER_FACTOR;
            }
            else{
                cout << "----扩容失败----" << endl;
                return ;
            }
    }
}

//空间利用率过低，缩容
void CapacityReduce(SqList &L)
{
    if (L.listsize == LIST_INIT_SIZE){
        return ;
    }
    //表长小于初始分配容量且表容量大于初始分配容量，表容量设置为初始分配容量
    if (L.length < LIST_INIT_SIZE && L.listsize > LIST_INIT_SIZE){        
        ElemType* newL = (ElemType *)realloc(L.elem, LIST_INIT_SIZE * sizeof(ElemType));
        if(newL != NULL){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize = LIST_INIT_SIZE;
        }
        else{
            cout << "----缩容失败----" << endl;
            return ;
        }
    }
    // 表长小于表容量一半,表容量大于初始分配容量的2倍，小于最大倍增容量,表容量减半
    else if (L.length < L.listsize / 2 && L.listsize > LIST_INIT_SIZE * MULTIPLIER_FACTOR && L.listsize <= MAX_DOUBLE_INCREMENT){
        ElemType* newL = (ElemType *)realloc(L.elem, (L.listsize / 2) * sizeof(ElemType));
        if(newL != NULL){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize /= 2;
        }
        else{
            cout << "----缩容失败----" << endl;
            return ;
        } 
    }
    //表容量大于最大倍增容量，表容量设置为最大倍增容量
    else if (L.listsize > MAX_DOUBLE_INCREMENT && L.length < MAX_DOUBLE_INCREMENT){
        ElemType* newL = (ElemType *)realloc(L.elem, (MAX_DOUBLE_INCREMENT) * sizeof(ElemType));
        if(newL != NULL){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize = MAX_DOUBLE_INCREMENT;
        }
        else{
            cout << "----缩容失败----" << endl;
            return ;
        }    

    }  
    //表长大于最大倍增容量，表容量大于表长+固定分配容量，表容量减去固定分配容量
    else if(L.length < L.listsize - FIXED_INCREMENT && L.length > MAX_DOUBLE_INCREMENT && L.listsize < MAX_INCREMENT){
        ElemType* newL = (ElemType *)realloc(L.elem, (L.listsize - FIXED_INCREMENT) * sizeof(ElemType));
        if(newL != NULL){       //申请内存成功,更新指针
            L.elem = newL;
            L.listsize -= FIXED_INCREMENT;
        }
        else{
            cout << "----缩容失败----" << endl;
            return ;
        }    
    }
}

//顺序表初始化
bool InitList(SqList &L)
{
    //顺序表不存在，创建新表
    if(L.elem != NULL){
        cout << "----顺序表已存在----" << endl;
        return false;
    }
    L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if(L.elem == NULL){
        return false;
    }
    assert(L.elem != NULL);  //判断指针是否有效
    //申请成功，初始化顺序表
    L.length = 0;   //表长初始为0
    L.listsize = LIST_INIT_SIZE;    //容量设置为初始容量

    return true;
}

//覆盖原字符串
void CoverList(SqList &L)
{
        //输入字符串
    char str[BUFFER_SIZE];     //定义缓冲区大小
    int len = 0;                //按单词大小申请内存，避免浪费空间
    if (fgets(str, BUFFER_SIZE, stdin) != NULL) {    
        char* token = strtok(str, " ,\n"); // 分割字符串，以空格或逗号分隔
        int pos = 0;
        while (token != NULL && pos < L.length) {
            CapacityExpansion(L);  //判断并扩容

            len = strlen(token);
            if (len > strlen(L.elem[pos].str)){    //如果新单词长度大于原来位置，重申请内存
                free(L.elem[pos].str);
                L.elem[pos].str = (char*)malloc(sizeof(len + 1));
            }
            strcpy(L.elem[pos].str, token);       //将单词复制到新节点中
            token = strtok(NULL, " ,\n"); // 继续分割
            pos ++;
        }
        while (token != NULL){
            CapacityExpansion(L);  //判断并扩容

            len = strlen(token);
            L.elem[pos].str = (char *)malloc(sizeof(len));
            strcpy(L.elem[pos].str, token);       //将单词复制到新节点中
            //将新节点加入到顺序表中
            token = strtok(NULL, " ,\n"); // 继续分割
            pos ++;
            L.length ++;
        }
    }
    //处理 fgets 失败的情况
    else{
        free(L.elem); // 释放头结点内存
        L.elem = NULL;
        cout << "输入错误或无法读取" << endl;
        return ;
    }
}

//字符串输入
void ListInput(SqList &L)
{
    //原顺序表存在，覆盖原表
    if (L.elem != NULL){
        cout << "----原顺序表存在, 将被覆盖----" << endl;
        CoverList(L);
        return ;
    }
    else{   //顺序表不存在，初始化顺序表
        if(!InitList(L)){
            cout << "顺序表初始化失败" << endl;
            return ;
        };    
    }
    //输入字符串
    char str[BUFFER_SIZE];     //定义缓冲区大小
    int len = 0;                //按单词大小申请内存，避免浪费空间
    if (fgets(str, BUFFER_SIZE, stdin) != NULL) {    
        char* token = strtok(str, " ,\n"); // 分割字符串，以空格或逗号分隔
        int pos = 0;
        while (token != NULL) {
            CapacityExpansion(L);  //判断并扩容

            len = strlen(token);
            L.elem[pos].str = (char *)malloc(sizeof(len));
            strcpy(L.elem[pos].str, token);       //将单词复制到新节点中
            //将新节点加入到顺序表中
            token = strtok(NULL, " ,\n"); // 继续分割
            pos ++;
            L.length++;
        }
    }
    //处理 fgets 失败的情况
    else{
        free(L.elem); // 释放头结点内存
        L.elem = NULL;
        cout << "输入错误或无法读取" << endl;
        return ;
    }
    cout << "顺序表创建成功" << endl;
}

//顺序表销毁
bool DestroyList(SqList &L)
{
    if (L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return false;
    }
    //先释放每个已申请内存的char指针，L.elem[0, ... , L.length - 1]
    for(int i = 0; i < L.length; i++){
        free(L.elem[i].str);
    }
    L.length = 0;
    L.listsize = 0;
    free(L.elem);   //释放顺序表
    L.elem = NULL;    //指针置空
    return true;
}

//清空顺序表
void ClearList(SqList &L)
{
    if(L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return ;
    }
    assert(L.elem != NULL);
    //释放单词的内存
    for(int i = 0; i < L.length; i++){
        free(L.elem[i].str);
    }
    //如果顺序表长度大于LIST_INIT_SIZE
    if (L.listsize > LIST_INIT_SIZE){
        ElemType* newL = (ElemType*)realloc(L.elem, LIST_INIT_SIZE * sizeof(ElemType));    //将容量设置为LIST_INIT_SIZE,避免浪费空间
        if (!newL){
            cout << "---清空后缩容失败----" << endl;
            return ;
        }
        L.elem = newL;
        L.listsize = LIST_INIT_SIZE;   //设置为初始容量大小
    }
    L.length = 0;    //长度置为0
    
}

//判断顺序表是否为空
bool ListEmpty(SqList L)
{
    if(L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return false;
    }
    assert(L.elem != NULL);
    if (L.length > 0)
    {
        return true;
    }
    return false;
}

//获取顺序表长度
int ListLength(SqList L)
{
    if(L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return -1;
    }
    assert(L.elem != NULL);
    return L.length;
}

//获取第i个元素
bool GetElem(SqList L, int i, ElemType &e)
{
    if(L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return false;
    }
    assert(L.elem != NULL);
    if (i < 1 || i > L.length){   //无效查询
        cout << "越界" << endl;
        return false;
    }
    e = L.elem[i - 1];
    return true;
}

//查找元素e的位置
int LocateElem(SqList L, ElemType e, Compare compare)
{
    if(L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return -1;
    }
    assert(L.elem != NULL);
    for (int i = 0; i < L.length; i++){
        if ((compare)(L.elem[i], e)){
            return i + 1;
        }
    }
    return -1;
}

//查找元素e的直接前驱
bool PriorElem(SqList L, ElemType cur_e, ElemType &pre_e)
{
    if(L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return false;
    }
    assert(L.elem != NULL);
    int pos = LocateElem(L, cur_e, strcompare);
    if (pos != -1 && pos - 2 >= 0)
    {
        pre_e = L.elem[pos - 2];
        return true;
    }
    return false;
}

//查找元素e直接后驱
bool NextElem(SqList L, ElemType cur_e, ElemType &next_e)
{
    if(L.elem == NULL){
        cout << "顺序表不存在" << endl;
        return false;
    }
    assert(L.elem !=NULL);
    int pos = LocateElem(L, cur_e, strcompare);
    if (pos != -1 && pos < L.length)
    {
        next_e = L.elem[pos];
        return true;
    }
    return false;
}

//插入节点
bool ListInsert(SqList &L, int i, ElemType e)
{
    if (i < 1 || i > L.length + 1){
        return false;
    }
    CapacityExpansion(L);   //判断并扩容

    ElemType* q = L.elem + i - 1;
    for (ElemType* p = L.elem + L.length - 1; p >= q; p--){   //后移节点L.elem[i-1, ..., L.length - 1]
        *(p + 1) = *p; 
    }
    *q = e;
    L.length ++;   
    return true;
}


//删除第i个节点
bool ListDelete(SqList &L, int i, ElemType &e)
{
    if (i < 1 || i > L.length){
        return false;
    }
    ElemType* q = L.elem + L.length - 1;
    ElemType* p = L.elem + i - 1;
    e = *p;
    free(p);
    p = NULL;
    for (++p; p <= q; p++){   //前移节点L.elem[i - 1, L.length - 1]
        *(p - 1) = *p; 
    }
    L.length --;
    return true;
}

//打印顺序表
void ListPrint(SqList L)
{
    for (int i = 0; i < L.length - 1; i++){
        cout << L.elem[i].str << " " ;
    }
    cout << L.elem[L.length - 1].str << endl;
}

void ListReverse(SqList &L)
{
    return ;
}

bool IsPalindrome(SqList L)
{
    return true;
}

bool SearchSubList(SqList L, SqList pattern)
{
    return true;
}

void MergeList(SqList &L, SqList pattern)
{
    return ;
}