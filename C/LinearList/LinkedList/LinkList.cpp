#include"LinkList.h"

//比较ElemType大小
int strcompare(ElemType e1, ElemType e2)
{
    return strcmp(e1, e2);
}

//求数据长度
int datalen(ElemType e)
{
    return strlen(e) + 1;
}

//复制节点数据
void datacpy(ElemType &dst, ElemType src)
{
    if (src == NULL || dst == NULL){
        fprintf(stderr, "节点不存在: DataExit Error\n"); 
        return ;
    }
    strcpy(dst, src);
}

//复制节点
void Nodecpy(LinkList &dst, LinkList src)
{
    if (src == NULL || dst == NULL){
        fprintf(stderr, "复制失败: NodeCopy Error\n"); 
        return ;
    }
    if(src->data == NULL){     //src数据为空
        return ;
    }
    if(dst->data == NULL){
        dst->data = (ElemType)malloc(datalen(src->data));
    }
    if(dst->data != NULL){
        datacpy(dst->data, src->data);
    }
}

DataCompare compare = strcompare;     //节点数据比较函数

//创建节点
void CreateNode(LinkList &node, size_t size)
{
    node = (LinkList)malloc(sizeof(LNode));    //申请动态内存，创建新节点
    if(node == NULL){
        fprintf(stderr, "节点创建失败: CreateNode Error\n");
        return ;
    }
    node->next = NULL;
    node->data = NULL;
    if (size > 0){
        node->data = (ElemType)malloc(size);            //动态申请单词内存
    }
}

//将数据放入链表中
void CreateList(LinkList &Head, ElemType str)
{
    if(Head == NULL){
        fprintf(stderr, "链表不存在: ListExit Erro\n");
        return ;
    }
    int len = 0;                //按单词大小申请内存，避免浪费空间

    char* token = strtok(str, " ,\n"); // 分割字符串，以空格或逗号分隔
    LinkList cur = Head;
    while (token != NULL) {
        len = strlen(token);
        LinkList newnode;    //申请动态内存，创建新节点
        CreateNode(newnode, (len + 1));
        if(newnode == NULL){    //申请失败
            fprintf(stderr, "链表创建失败: CreateList Error\n");
            return ;
        }
        datacpy(newnode->data, token);       //将单词复制到新节点中
        //将新节点加入到链表末尾
        cur->next = newnode;
        cur = cur->next;   //节点后移
        token = strtok(NULL, " ,\n"); // 继续分割
    }
}

//覆盖非空链表
void CoverList(LinkList &Head, ElemType str)
{
    //原链表不存在或者存在但链表为空
    if(Head == NULL || (Head != NULL && Head->next == NULL)){ 
        fprintf(stderr, "链表不存在: ListExit Erro\n");  
        return ;
    }

    int len = 0;                //按单词大小申请内存，避免浪费空间   
    char* token = strtok(str, " ,\n?？ ： ， ！"); // 分割字符串，以空格或逗号分隔
    LinkList cur = Head->next;
    LinkList phead = NULL;  //保存最后一个覆盖节点

    //覆盖原有节点
    while (token != NULL && cur) {
        len = strlen(token);
        //释放原节点单词空间，申请新空间
        ElemType newdata = (ElemType)malloc(len + 1);
        if (newdata == NULL){
            fprintf(stderr, "链表覆盖失败: CreateCover Error\n");
            return ;
        }
        free(cur->data);
        cur->data = newdata;
        strcpy(cur->data, token);       //将单词复制到节点中

        phead = cur;
        cur = cur->next;    //节点后移
        token = strtok(NULL, " ,\n"); // 继续分割
    }
    
    //如果少于原来的链表长度，缩容
    while(cur){
        DestroyList(cur);
        phead->next = NULL;
    }
    //输入单词数超过链表长度,扩容
    while (token != NULL) {
        len = strlen(token);
        LinkList newnode;    //申请动态内存，创建新节点
        CreateNode(newnode, (len + 1));
        if(newnode == NULL){
            fprintf(stderr, "链表扩容失败: ListExpansion Error\n");
            return ;
        }
        strcpy(newnode->data, token);       //将单词复制到新节点中
        //将新节点加入到链表中
        phead->next = newnode;
        token = strtok(NULL, " ,\n"); // 继续分割
        phead = phead->next;   //节点后移
    }
}


//从键盘输入链表内容(尾插)
void CreateListFromInput(LinkList &Head)
{
    //输入字符串
    char str[BUFFER_SIZE];     //定义缓冲区大小
    if (fgets(str, BUFFER_SIZE, stdin) != NULL) {  //读取成功
        if (Head != NULL && Head->next){   // 链表存在且不为空，直接覆盖原链表
            CoverList(Head, str);
            return ;
        }

        //链表不存在，创建新链表
        if(Head == NULL){
            CreateNode(Head, 0);   //头节点
            if (Head == NULL){                      //判断链表是否创建成功
                fprintf(stderr, "链表创建失败: CreateList Error\n");
                return ; 
            }    
        }
        CreateList(Head, str);   
    }
    // fgets失败
    else{
        fprintf(stderr, "输入失败: KeyboardInput error\n");
        return ;
    }
}

//从文件读入链表内容(尾插)
void CreateListFromFile(LinkList &Head, const char* path)
{
    FILE *file = fopen(path, "r"); // 打开文件用于读取
    if (file == NULL) {
        fprintf(stderr, "文件读取失败: FileOpen error\n");  // 如果文件不能打开
        return ;
    }
    //输入字符串
    char str[BUFFER_SIZE];     //定义缓冲区大小
    if (fgets(str, BUFFER_SIZE, file) != NULL) {  //读取成功
        if (Head != NULL){   // 链表存在，直接覆盖原链表
            CoverList(Head, str);
            return ;
        }

        //链表不存在，创建新链表
        CreateNode(Head, 0);   //头节点
        if (Head == NULL){                      //判断链表是否创建成功
            fprintf(stderr, "链表创建失败: CreateList Error\n");
            return ;
        }
        //assert(Head != NULL);    
        CreateList(Head, str);
    }
    // fgets失败
    else{
        fprintf(stderr, "文件读取失败: FileOpen error\n");
        return ;
    }
    fclose(file);
}

//销毁链表
void DestroyList(LinkList &Head)
{
    if (Head == NULL){    //链表不存在
        // fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return ;
    }
    LinkList next = NULL, cur = Head->next;
    while(cur)
    {
        next = cur->next;
        free(cur->data);      //先释放ElemType的char指针指向的内存
        cur->data = NULL;    
        free(cur);          //释放当前节点
        cur = next;   //节点后移
    }
    
    free(Head);   //释放头结点
    Head = NULL;    //防止指针悬空
}

//清空链表，保留头指针
void ClearList(LinkList &Head)
{
    if (Head == NULL){
        // fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return ;
    }
    LinkList next = NULL, cur = Head->next;
    while(cur)
    {
        next = cur->next;
        free(cur->data);        //释放单词的内存
        cur->data = NULL;
        free(cur);          //释放节点
        cur = next;       //节点后移
    }
}

//判断链表是否为空
bool ListEmpty(LinkList Head)
{
    if (Head == NULL){   //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    if(Head->next != NULL){     //只需判断头结点下一个节点是否存在
        return true;
    }
    return false;
}

//获取链表长度
int ListLength(LinkList Head)
{
    if (Head == NULL){     //链表不存在，返回-1
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return -1;
    }
    LinkList cur = Head->next;
    int count = 0;
    while(cur)
    {
        count ++;
        cur = cur->next;   //指针后移
    }
    return count;
}

//获取第i个节点
bool GetElem(LinkList Head, int i, ElemType &e)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    assert(Head != NULL);
    LinkList cur = Head->next;
    int pos = 0;       //记录位置
    while(cur && pos < i)    //i越界结束循环
    {
        pos ++;
        if (pos == i){      //找到第i个节点
            e = cur->data;  //返回值到e中
            return true;
        }
        cur = cur->next;        //指针后移
    }
    return false;
}

//找到数据项e的位置
int LocateElem(LinkList Head, ElemType e, DataCompare compare)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return -1;
    }
    LinkList cur = Head->next;
    int pos = 0;
    while(cur)
    {
        pos ++;
        if (!compare(e, cur->data)){   //找到数据位置
            return pos;
        }
        cur = cur->next;        //指针后移
    }
    return -1;      //找不到返回-1
}


//根据节点数据找到节点前驱数据
bool PriorElem(LinkList Head, ElemType cur_e, ElemType &pre_e)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    LinkList cur = Head->next, pre = Head;
    while(cur)
    {
        if(!compare(cur->data, cur_e)){     //和当前节点比较
            pre_e = pre->data;
            return true;
        }
        pre = cur;      //节点后移
        cur = pre->next;
    }
    return false;
}

//根据节点数据找到后继节点数据
bool NextElem(LinkList Head, ElemType cur_e, ElemType &next_e)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    LinkList cur = Head->next;
    while(cur->next)
    {
        if (!compare(cur_e, cur->data)){    //如果找到
            next_e = cur->next->data;         //返回后继节点数据
            return true;
        }
        cur = cur->next;    //节点后移
    }
    return false;
}

//在第i个节点处插入元素
bool ListInsert(LinkList &Head, int i, ElemType e)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = Head;
    int pos = 0;
    while(prev && pos < i)
    {
        if (pos == i - 1){
            LinkList newnode;
            CreateNode(newnode, datalen(e));   //创建插入节点
            if(newnode == NULL){
                fprintf(stderr, "链表插入失败: ListInsert Erro\n"); 
                break;
            }
            strcpy(newnode->data, e);
            newnode->next = prev->next;         //插入节点
            prev->next = newnode;
            return true;
        }
        prev = prev->next;    //节点后移
        pos ++;
    }
    if(prev == NULL){    //越界
        fprintf(stderr, "下标越界: Index Erro\n"); 
    }
    return false;
}


//在指定位置插入一个或多个单词
bool ListInsertWords(LinkList &Head, int i, LinkList result)
{
    if (Head == NULL || result == NULL){
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = Head;
    int pos = 1;
    while(prev && pos < i - 1)    //找到第i-1个节点
    {
        prev = prev->next;    //节点后移
        pos ++;
    }
    if(prev == NULL){    //越界
        fprintf(stderr, "下标越界: Index Erro\n"); 
        return false;
    }
    
    LinkList r_tail = result->next;
    while(r_tail->next){
        r_tail = r_tail->next;
    }
    //插入单词
    r_tail->next = prev->next;
    prev->next = r_tail;

    return true;
}

//删除第i个节点
bool ListDelete(LinkList &Head, int i, ElemType &e)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = Head;    //找直接前驱
    int pos = 0;
    while(prev->next)
    {
        if (pos == i - 1){
            LinkList dst = prev->next;   //保存第i个节点
            prev->next = prev->next->next;  //更改指针
            e = dst->data;
            free(dst);
            dst = NULL;
            return true;
        }
        prev = prev->next;        //节点后移
        pos ++;
    }
    if(prev->next == NULL){    //越界
        fprintf(stderr, "下标越界: Index Erro\n");
    }
    return false;
}


//在指定位置删除一个或多个单词
bool ListDeleteWords(LinkList &Head, int i, int n, LinkList &result)
{   
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = Head;    
    int pos = 1;
    while(prev->next && pos < i - 1)  //找直接前驱
    {
        prev = prev->next;        //节点后移
        pos ++;
    }
    LinkList tmp = NULL;
    LinkList r_cur = result;
    while(prev->next && --n){
        tmp = prev->next;     //保存指针
        prev->next = prev->next->next;   //删除单词
        r_cur->next = tmp;     //单词放到result链表中
        r_cur = tmp;
    }
    return true;
}

//打印链表
void ListPrint(LinkList Head)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return ;
    }
    LinkList cur = Head->next;
    while(cur)
    {
        cout << cur->data << " ";   //打印单词
        cur = cur->next;        //节点后移
    }
    cout << endl;
}

//链表倒置
void ListReverse(LinkList &Head)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return ;
    }
    if (Head->next == NULL || Head->next->next == NULL){   //空链表或单节点直接返回
        return ;
    } 
    LinkList prev = NULL;
    LinkList cur = Head->next;
    LinkList Lnext = NULL;
    while(cur){
        Lnext = cur->next;
        cur->next = prev;   //改变指针方向
        prev = cur;     //节点后移
        cur = Lnext;
    }
    Head->next = prev;  //头结点指向新的首节点

}

//将某一段子串倒置并返回倒置后的的新字符串
LinkList ListRangeReverse(LinkList Head, int start, int end)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return NULL;
    }
    int pos = 1;
    LinkList newHead;
    CreateNode(newHead, 0);
    if(newHead == NULL){
        fprintf(stderr, "倒置失败: ListReverse Erro\n"); 
        return NULL;
    }

    newHead->next = NULL;
    LinkList cur = Head->next;
    while(cur && pos < start){    //找到第start个节点
        pos ++;
        cur = cur->next;
    }
    while(cur && pos <= end && pos >= start)
    {
        ListInsert(newHead, 1, cur->data);  //头插，倒置子串
        cur = cur->next;        //指针后移
        pos ++;
    }

    return newHead;
}

//判断是否是回文串(词级别)
bool IsPalindrome(LinkList Head)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    int length = ListLength(Head);      //字符串长度
    int start = length / 2 + 1;        //倒置开始位置
    LinkList R_Head = ListRangeReverse(Head, start, length);        //字符串后半段的倒置字符串
    LinkList R_cur = R_Head->next;
    LinkList cur = Head->next;
    for (int i = 1 ; i < start; i++){
        if (compare(cur->data, R_cur->data)){      //如果不相等
            return false;
        }  
        cur = cur->next;
        R_cur = R_cur->next;    //指针后移
    }
    return true;
}

//查找子串
bool SearchSubList(LinkList Head, LinkList pattern)
{
    if (Head == NULL){      //链表不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return false;
    }
    if (pattern == NULL || pattern->next == NULL){   //匹配串不存在或者为空
        return false;
    }
    LinkList left = Head->next, right = Head->next;    //字符串左右匹配指针
    LinkList subright = pattern->next;   //匹配串当前匹配的位置
    while(right && subright){
        if (compare(right->data, subright->data)){  //不相等，Head字符串后移一个节点，重新匹配子串
            left = left->next;
            right = left;
            subright = pattern->next;
            continue;
        }
        right = right->next;
        subright = subright->next;      //节点后移
    }
    if (!subright){     //子串匹配完成
        return true;
    }
    return false;
}

//Head2合并到Head1，保持非递减
void MergeList(LinkList &Head1, LinkList Head2)
{
    if (Head1 == NULL || Head2 == NULL){      //链表1或2不存在
        fprintf(stderr, "链表不存在: ListExit Erro\n"); 
        return ;
    }

    LinkList next1 = Head1->next;    //链表1尾结点
    LinkList next2 = Head2->next;     //链表2尾结点
    LinkList cur = Head1;               //新链表头结点
    while(next1 && next2){
        if(compare(next1->data, next2->data) <= 0){      //tail1小于等于tail2, 链表1节点加入新链表
            cur->next = next1;
            cur = next1;
            next1 = next1->next;
        }
        else{                               //tail2小于tail1,链表2节点加入新链表
            cur->next = next2;
            cur = next2;
            next2 = next2->next;
        }
    }
    if(next1){      //如果Head1序列未结束
        cur->next = next1;
    }
    if (next2){      //如果Head2序列未结束
        cur->next = next2;
    }
}



/*

//两个非递减序列合并成一个新的非递减序列,不释放原来两个链表空间,合并后只需要释放两个链表的头结点
void NewMergeList(LinkList &Head1, LinkList Head2)
{
    
    
    
    if(Head1 == NULL || Head2 == NULL){
        fprintf(stderr, "链表不存在: ListExit Error\n"); 
        return NULL;
    }

    LinkList MergeHead;
    CreateNode(MergeHead);
    if (MergeHead == NULL){
        fprintf(stderr, "合并失败: MergeList Error\n"); 
        return NULL;
    }
    MergeHead->next = NULL;
    
    //链表1/2都存在
    LinkList next1 = Head1->next;    //链表1尾结点
    LinkList next2 = Head2->next;
    LinkList cur = MergeHead;
    while(next1 && next2)
    {
        if(compare(next1->data, next2->data) <= 0){      //tail1小于等于tail2,链表1节点加入新链表
            cur->next = next1;
            cur = next1;
            next1 = next1->next;
        }
        else{                               //tail2小于tail1,链表2节点加入新链表
            cur->next = next2;
            cur = next2;
            next2 = next2->next;
        }
    }
    if(next1){      //如果Head1序列未结束
        cur->next = next1;
    }
    if (next2){      //如果Head2序列未结束
        cur->next = next2;
    }
}
*/


//两个非递减序列合并成一个新的非递减序列,合并后需释放两个链表空间
LinkList NewMergeList(LinkList Head1, LinkList Head2)
{   
    if(Head1 == NULL || Head2 == NULL){       //合并不存在的链表非法
        fprintf(stderr, "链表不存在: ListExit Error\n"); 
        return NULL;
    }

    LinkList MergeHead;     //申请失败
    CreateNode(MergeHead, 0);
    if (MergeHead == NULL){
        fprintf(stderr, "合并失败: MergeList Error\n"); 
        return NULL;
    }
    
    LinkList next1 = Head1->next;    //链表1尾结点
    LinkList next2 = Head2->next;
    LinkList cur = MergeHead;
    int pos = 1;   //插入的位置
    while(next1 && next2)
    {
        if(compare(next1->data, next2->data) <= 0){      //tail1小于等于tail2,,链表1节点加入新链表
            LinkList newnode;
            CreateNode(newnode, 0);
            if (newnode == NULL){
                fprintf(stderr, "合并失败: MergeList Error\n"); 
                DestroyList(MergeHead);    //销毁，避免内存泄露
                return NULL;
            }

            Nodecpy(newnode, next1);
            cur->next = newnode;    //新节点加入合并链表
            cur = cur->next;        //后移指针
            next1 = next1->next;
        }
        else{                               //tail2小于tail1,链表2节点加入新链表
            LinkList newnode;
            CreateNode(newnode, 0);
            if (newnode == NULL){
                fprintf(stderr, "合并失败: MergeList Error\n"); 
                DestroyList(MergeHead);    //销毁，避免内存泄露
                return NULL;
            }

            Nodecpy(newnode, next2);
            cur->next = newnode;    //新节点加入合并链表
            cur = cur->next;        //后移指针
            next2 = next2->next;
        }
    }
    while(next1){      //如果Head1序列未结束
         LinkList newnode;
        CreateNode(newnode, 0);
        if (newnode == NULL){
            fprintf(stderr, "合并失败: MergeList Error\n"); 
            DestroyList(MergeHead);    //销毁，避免内存泄露
            return NULL;
        }

        Nodecpy(newnode, next1);
        cur->next = newnode;    //新节点加入合并链表
        cur = cur->next;        //后移指针
        next1 = next1->next;
    }
    while(next2){      //如果Head2序列未结束
        LinkList newnode;
        CreateNode(newnode, 0);
        if (newnode == NULL){
            fprintf(stderr, "合并失败: MergeList Error\n"); 
            DestroyList(MergeHead);    //销毁，避免内存泄露
            return NULL;
        }

        Nodecpy(newnode, next2);
        cur->next = newnode;    //新节点加入合并链表
        cur = cur->next;        //后移指针
        next2 = next2->next;
    }
    return MergeHead;
}

//寻找两个链表的不同子串
LinkList SearchDiff(LinkList Head1, LinkList Head2)
{
    if (Head1 == NULL){
        cout << "链表1不存在" << endl;
        return NULL;
    }
    if (Head2 == NULL){
        cout << "链表2不存在" << endl;
        return NULL;
    }
    LinkList diff = NULL;
    return diff;
}