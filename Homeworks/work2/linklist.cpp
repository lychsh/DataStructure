#include"linklist.h"
#define CHINESE 2
#define ENGLISH 1

/*
----------------------------------------------基本操作-----------------------------------------------------
*/

//比较数据大小
int datacmp(elemtype e1, elemtype e2)
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

//复制节点
void nodecpy(linklist &dst, linklist src)
{
    if (src == nullptr || dst == nullptr){
        fprintf(stderr, "复制失败: NodeCopy Error\n"); 
        return ;
    }
    dst->data = src->data;
}

//创建节点
void create_node(linklist &node, elemtype e)
{
    node = new LNode;    //申请动态内存，创建新节点
    if(node == nullptr){
        fprintf(stderr, "节点创建失败: CreateNode Error\n");
        return ;
    }
    node->next = nullptr;
    if(!e.empty()){
        node->data = e;    //赋值
    }
    
}

//销毁链表
void destroy_list(linklist &head)
{
    if (head == nullptr){    //链表不存在
        //fprintf(stderr, "链表不存在: listExit Error\n"); 
        return ;
    }
    linklist next = nullptr, cur = head->next;
    while(cur)
    {
        next = cur->next;   
        delete cur;          //释放当前节点
        cur = next;   //节点后移
    }
    
    delete head;   //释放头结点
    head = nullptr;    //防止指针悬空
}

//清空链表，保留头指针
void clear_list(linklist &head)
{
    if (head == nullptr){     //链表不存在
        fprintf(stderr, "链表不存在: listExit Error\n"); 
        return ;
    }
    linklist cur = head->next;
    while(cur)
    {
        cur->data = "";    //清空数据
        cur = cur->next;       //节点后移
    }
}

//判断链表是否为空
bool list_empty(linklist head)
{
    if (head == nullptr){   //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    if(head->next != nullptr){     //只需判断首元节点
        return false;
    }
    return true;
}

//获取链表长度
int list_length(linklist head)
{
    if (head == nullptr){     //链表不存在，返回-1
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return -1;
    }
    linklist cur = head->next;
    int count = 0;
    while(cur)
    {
        count ++;
        cur = cur->next;   //指针后移
    }
    return count;
}

//获取第i个节点
bool get_elem(linklist head, int i, elemtype &e)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    linklist cur = head->next;
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
int locate_elem(linklist head, elemtype e, bool (*compare)(elemtype, elemtype))
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return -1;
    }
    linklist cur = head->next;
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
bool prior_elem(linklist head, elemtype cur_e, elemtype &pre_e)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    linklist cur = head->next, pre = head;
    while(cur)
    {
        if(!datacmp(cur->data, cur_e)){     //和当前节点比较
            pre_e = pre->data;
            return true;
        }
        pre = cur;      //节点后移
        cur = pre->next;
    }
    return false;
}

//根据节点数据找到后继节点数据
bool next_elem(linklist head, elemtype cur_e, elemtype &next_e)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    if(!head->next || !head->next->next){   //链表为空或者只有一个节点
        return false;
    }
    linklist cur = head->next;
    while(cur->next)
    {
        if (!datacmp(cur_e, cur->data)){    //如果找到
            next_e = cur->next->data;         //返回后继节点数据
            return true;
        }
        cur = cur->next;    //节点后移
    }
    return false;
}

//在第i个节点处插入元素
bool list_insert(linklist &head, int i, elemtype e)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    linklist prev = head;
    int pos = 0;
    while(prev && pos < i)
    {
        if (pos == i - 1){
            linklist newnode;
            create_node(newnode, e);   //创建插入节点
            if(newnode == nullptr){
                fprintf(stderr, "链表插入失败: listInsert Erro\n"); 
                break;
            }
            newnode->next = prev->next;         //插入节点
            prev->next = newnode;
            return true;
        }
        prev = prev->next;    //节点后移
        pos ++;
    }
    if(prev == nullptr){    //越界
        fprintf(stderr, "下标越界: Index Erro\n"); 
    }
    return false;
}

//删除第i个节点
bool list_delete(linklist &head, int i, elemtype &e)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    linklist prev = head;    //找直接前驱
    int pos = 0;
    while(prev->next)
    {
        if (pos == i - 1){
            linklist dst = prev->next;   //保存第i个节点
            prev->next = prev->next->next;  //更改指针
            e = dst->data;
            delete dst;
            dst = nullptr;
            return true;
        }
        prev = prev->next;        //节点后移
        pos ++;
    }
    if(prev->next == nullptr){    //越界
        fprintf(stderr, "下标越界: Index Erro\n");
    }
    return false;
}

//打印链表
void list_print(linklist head)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return ;
    }
    linklist cur = head->next;
    while(cur)
    {
        std::cout << cur->data << " ";   //打印单词
        cur = cur->next;        //节点后移
    }
    std::cout << std::endl;
}

/*
--------------------------------------------初级功能-----------------------------------------------------
*/

//返回从start开始第一个分隔符的位置
elemtype::size_type find_first_delimiter(elemtype str, elemtype::size_type size, elemtype::size_type start, int &language)
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


//将数据放入链表中
void create_list(linklist &head, elemtype str)
{
    if(head == nullptr){
        fprintf(stderr, "链表不存在: listExit Erro\n");
        return ;
    }
    linklist cur = head;
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word; 
    int language = 1;     //分隔符中文还是英文，决定下一次查找跳过一个字节还是两个
    if(size <= 0){      //字符串为空，置空链表
        clear_list(head);
        return ;
    }
    while(start <= (size - 1)){
        end = find_first_delimiter(str, size, start, language);
        if (end != start){
            word = str.substr(start, end - start);
            linklist newnode;           //申请动态内存，创建新节点
            create_node(newnode, word);
            if(word == "" || word == " "){
                start = start + language; 
                continue;
            }
            if(newnode == nullptr){            //申请失败
                fprintf(stderr, "链表创建失败: Createlist Error\n");
                destroy_list(head);
                return ;
            }
            //将新节点加入到链表末尾
            cur->next = newnode;
            cur = cur->next;   //节点后移
        }
        start = end + language;
    }
}

//覆盖非空链表
void cover_list(linklist &head, elemtype str)
{
    //原链表不存在或者存在但链表为空
    if(head == nullptr || head->next == nullptr){ 
        fprintf(stderr, "链表不存在: listExit Erro\n");  
        return ;
    }
    linklist cur = head->next;
    linklist phead = nullptr;  //保存最后一个覆盖节点
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word; 
    int language = 0;

    if(size <= 0){      //字符串为空，置空链表
        clear_list(head);
        return ;
    }
    while((start <= size - 1) && cur){
        end = find_first_delimiter(str, size, start, language);
        if (end != start){
            word = str.substr(start, end - start);
            if(word == "" || word == " "){
                start = end + language; 
                continue;
            }
            cur->data = word;
            phead = cur;
            cur = cur->next;   //节点后移
        }
        start = end + language; 
    }
    //如果少于原来的链表长度，缩容
    while(cur){
        destroy_list(cur);
        phead->next = nullptr;
    }

    //输入单词数超过链表长度,扩容
    if(start < size){
        create_list(phead, str.substr(start));
    }
   
}


//从键盘输入链表内容(尾插)
void create_list_from_input(linklist &head)
{
    //输入字符串
    std::string str;     
    if (std::getline(std::cin, str)) {  //读取成功
        if (head != nullptr && head->next){   // 链表存在且不为空，直接覆盖原链表
            cover_list(head, str);
            return ;
        }

        //链表不存在，创建新链表
        if(head == nullptr){
            create_node(head, "");  //头节点
            if (head == nullptr){                      //判断链表是否创建成功
                fprintf(stderr, "链表创建失败: Createlist Error\n");
                return ; 
            }    
        }
        create_list(head, str);   
    }
    // 读取失败
    else{
        fprintf(stderr, "输入失败: KeyboardInput error\n");
        return ;
    }
}

//从文件读入链表内容(尾插)
void create_list_from_file(linklist &head, std::string path)
{
    std::ifstream file(path); // 打开文件用于读取
    if (!file.is_open()) {
        fprintf(stderr, "文件读取失败: FileOpen error\n");  // 如果文件不能打开
        return ;
    }
    //输入字符串
    elemtype str;     
    if (std::getline(file, str)) {  //读取成功
        if (head != nullptr && head->next){   // 链表存在且不为空，直接覆盖原链表
            cover_list(head, str);
            return ;
        }

        //链表不存在，创建新链表
        if(head == nullptr){
            create_node(head, "");   //头节点
            if (head == nullptr){                      //判断链表是否创建成功
                fprintf(stderr, "链表创建失败: Createlist Error\n");
                return ; 
            }    
        }
        create_list(head, str);   
    }
    file.close();
}

//在指定位置插入一个或多个单词
bool list_insert_words(linklist &head, int i, linklist words)
{
    if (head == nullptr || words == nullptr){
        fprintf(stderr, "链表不存在: listExit Error\n"); 
        return false;
    }
    linklist prev = head;     //插入位置前一个节点
    int pos = 0;
    while(prev && pos < i - 1)    //找到第i-1个节点
    {
        prev = prev->next;    //节点后移
        pos ++;
    }
    if(prev == nullptr){    //越界
        fprintf(stderr, "下标越界: Index Error\n"); 
        return false;
    }
    
    linklist r_tail = words->next;
    while(r_tail->next){
        r_tail = r_tail->next;
    }
    //插入单词
    r_tail->next = prev->next;
    prev->next = words->next;

    return true;
}

//在指定位置删除一个或多个单词
bool list_delete_words(linklist &head, int i, int n, linklist &result)
{   
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Error\n"); 
        return false;
    }
    linklist prev = head;    
    int pos =0;
    while(prev->next && pos < i - 1)  //找直接前驱
    {
        prev = prev->next;        //节点后移
        pos ++;
    }
    linklist tmp;
    linklist r_cur = result;
    while(prev->next && n--){
        tmp = prev->next;     //保存指针
        prev->next = prev->next->next;   //删除单词
        r_cur->next = tmp;     //删除的单词放到result链表中
        r_cur = tmp;
    }
    r_cur->next = nullptr;   //防止对原字符串造成影响
    if(n > 0){
        fprintf(stderr, "越界: Index Error\n"); 
        return false;
    }
    return true;
}

//链表倒置
void list_reverse(linklist &head)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return ;
    }
    if (head->next == nullptr || head->next->next == nullptr){   //空链表或单节点直接返回
        return ;
    } 
    linklist prev = nullptr;
    linklist cur = head->next;
    linklist Lnext = nullptr;     //倒置后字符串的首节点
    while(cur){
        Lnext = cur->next;
        cur->next = prev;   //改变指针方向
        prev = cur;     //节点后移
        cur = Lnext;
    }
    head->next = prev;  //头结点指向新的首节点

}

//将某一段子串倒置并返回倒置后的的新字符串
linklist list_range_reverse(linklist head, int start, int end)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return nullptr;
    }
    int pos = 1;
    linklist newhead;
    create_node(newhead, "");
    if(newhead == nullptr){
        fprintf(stderr, "倒置失败: listReverse Erro\n"); 
        return nullptr;
    }

    newhead->next = nullptr;
    linklist cur = head->next;
    while(cur && pos < start){    //找到第start个节点
        pos ++;
        cur = cur->next;
    }
    while(cur && pos <= end && pos >= start)
    {
        list_insert(newhead, 1, cur->data);  //头插，倒置子串
        cur = cur->next;        //指针后移
        pos ++;
    }

    return newhead;
}

//判断是否是回文串(词级别)
bool is_palindrome(linklist head)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    int length = list_length(head);      //字符串长度
    int start = length / 2 + 1;        //倒置开始位置
    linklist R_head = list_range_reverse(head, start, length);        //字符串后半段的倒置字符串
    linklist R_cur = R_head->next;
    linklist cur = head->next;
    for (int i = 1 ; i < start; i++){
        if (datacmp(cur->data, R_cur->data)){      //如果不相等
            return false;
        }  
        cur = cur->next;
        R_cur = R_cur->next;    //指针后移
    }
    return true;
}

//查找子串
bool search_sublist(linklist head, linklist pattern, linklist &result)
{
    if (head == nullptr){      //链表不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return false;
    }
    if (pattern == nullptr || pattern->next == nullptr){   //匹配串不存在或者为空
        return false;
    }
    linklist left = head->next, right = head->next;    //字符串左右匹配指针
    linklist subright = pattern->next;   //匹配串当前匹配的位置
    int pos = 1, count = 0;       //匹配的位置和次数
    while(right && left){
        while(right && subright){
            if (datacmp(right->data, subright->data)){  //不相等，head字符串后移一个节点，重新匹配子串
                left = left->next;
                right = left;
                pos ++;
                subright = pattern->next;
                continue;
            }
            right = right->next;
            subright = subright->next;      //节点后移
            }
        if (!subright){     //子串匹配完成
            count++;
            list_insert(result, count, std::to_string(pos));   //记录位置
            subright = pattern->next;
        }

        if(!left){     //主串匹配完
           break;
        }
        left = left->next;     //主串后移继续匹配
        right = left;
        pos ++;
            
    }
    if(count > 0){    //至少匹配一次
        return true;
    }
    return false;
}

//head2合并到head1，保持非递减
void merge_list(linklist &head1, linklist head2)
{
    if (head1 == nullptr || head2 == nullptr){      //链表1或2不存在
        fprintf(stderr, "链表不存在: listExit Erro\n"); 
        return ;
    }

    linklist next1 = head1->next;    //链表1尾结点
    linklist next2 = head2->next;     //链表2尾结点
    linklist cur = head1;               //新链表头结点
    while(next1 && next2){
        if(datacmp(next1->data, next2->data) <= 0){      //tail1小于等于tail2, 链表1节点加入新链表
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
    if(next1){      //如果head1序列未结束
        cur->next = next1;
    }
    if (next2){      //如果head2序列未结束
        cur->next = next2;
    }
}



/*

//两个非递减序列合并成一个新的非递减序列,不释放原来两个链表空间,合并后只需要释放两个链表的头结点
void NewMergelist(linklist &head1, linklist head2)
{
    
    if(head1 == nullptr || head2 == nullptr){
        fprintf(stderr, "链表不存在: listExit Error\n"); 
        return nullptr;
    }

    linklist Mergehead;
    CreateNode(Mergehead, "");
    if (Mergehead == nullptr){
        fprintf(stderr, "合并失败: Mergelist Error\n"); 
        return nullptr;
    }
    Mergehead->next = nullptr;
    
    //链表1/2都存在
    linklist next1 = head1->next;    //链表1尾结点
    linklist next2 = head2->next;
    linklist cur = Mergehead;
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
    if(next1){      //如果head1序列未结束
        cur->next = next1;
    }
    if (next2){      //如果head2序列未结束
        cur->next = next2;
    }
}
*/


//两个非递减序列合并成一个新的非递减序列,合并后需释放两个链表空间
linklist new_merge_list(linklist head1, linklist head2)
{   
    if(head1 == nullptr || head2 == nullptr){       //合并不存在的链表非法
        fprintf(stderr, "链表不存在: listExit Error\n"); 
        return nullptr;
    }

    linklist mergehead;     //申请失败
    create_node(mergehead, "");
    if (mergehead == nullptr){
        fprintf(stderr, "合并失败: Mergelist Error\n"); 
        return nullptr;
    }
    
    linklist next1 = head1->next;    //链表1尾结点
    linklist next2 = head2->next;
    linklist cur = mergehead;
    int pos = 1;   //插入的位置
    while(next1 && next2)
    {
        if(datacmp(next1->data, next2->data) <= 0){      //tail1小于等于tail2,,链表1节点加入新链表
            linklist newnode;
            create_node(newnode, next1->data);
            if (newnode == nullptr){
                fprintf(stderr, "合并失败: Mergelist Error\n"); 
                destroy_list(mergehead);    //销毁，避免内存泄露
                return nullptr;
            }
            cur->next = newnode;    //新节点加入合并链表
            cur = cur->next;        //后移指针
            next1 = next1->next;
        }
        else{                               //tail2小于tail1,链表2节点加入新链表
            linklist newnode;
            create_node(newnode, next2->data);
            if (newnode == nullptr){
                fprintf(stderr, "合并失败: Mergelist Error\n"); 
                destroy_list(mergehead);    //销毁，避免内存泄露
                return nullptr;
            }

            cur->next = newnode;    //新节点加入合并链表
            cur = cur->next;        //后移指针
            next2 = next2->next;
        }
    }
    while(next1){      //如果head1序列未结束
         linklist newnode;
        create_node(newnode, next1->data);
        if (newnode == nullptr){
            fprintf(stderr, "合并失败: Mergelist Error\n"); 
            destroy_list(mergehead);    //销毁，避免内存泄露
            return nullptr;
        }

        cur->next = newnode;    //新节点加入合并链表
        cur = cur->next;        //后移指针
        next1 = next1->next;
    }
    while(next2){      //如果head2序列未结束
        linklist newnode;
        create_node(newnode, next2->data);
        if (newnode == nullptr){
            fprintf(stderr, "合并失败: Mergelist Error\n"); 
            destroy_list(mergehead);    //销毁，避免内存泄露
            return nullptr;
        }

        cur->next = newnode;    //新节点加入合并链表
        cur = cur->next;        //后移指针
        next2 = next2->next;
    }
    return mergehead;
}
