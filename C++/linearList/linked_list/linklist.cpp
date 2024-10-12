#include"linklist.h"
#define CHINESE 2
#define ENGLISH 1

/*
----------------------------------------------��������-----------------------------------------------------
*/

//�Ƚ����ݴ�С
int datacmp(elemtype e1, elemtype e2)
{
    if (e1.length() > e2.length()){    //e1 > e2 ����1
        return 1;
    }
    else if(e1.length() < e2.length()){  //e1 < e2 ����-1
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
    return 0;     //e1 = e2 ����0
}

//���ƽڵ�
void nodecpy(linklist &dst, linklist src)
{
    if (src == nullptr || dst == nullptr){
        fprintf(stderr, "����ʧ��: NodeCopy Error\n"); 
        return ;
    }
    dst->data = src->data;
}

//�����ڵ�
void create_node(linklist &node, elemtype e)
{
    node = new LNode;    //���붯̬�ڴ棬�����½ڵ�
    if(node == nullptr){
        fprintf(stderr, "�ڵ㴴��ʧ��: CreateNode Error\n");
        return ;
    }
    node->next = nullptr;
    if(!e.empty()){
        node->data = e;    //��ֵ
    }
    
}

//��������
void destroy_list(linklist &head)
{
    if (head == nullptr){    //��������
        //fprintf(stderr, "��������: listExit Error\n"); 
        return ;
    }
    linklist next = nullptr, cur = head->next;
    while(cur)
    {
        next = cur->next;   
        delete cur;          //�ͷŵ�ǰ�ڵ�
        cur = next;   //�ڵ����
    }
    
    delete head;   //�ͷ�ͷ���
    head = nullptr;    //��ָֹ������
}

//�����������ͷָ��
void clear_list(linklist &head)
{
    if (head == nullptr){     //��������
        fprintf(stderr, "��������: listExit Error\n"); 
        return ;
    }
    linklist cur = head->next;
    while(cur)
    {
        cur->data = "";    //�������
        cur = cur->next;       //�ڵ����
    }
}

//�ж������Ƿ�Ϊ��
bool list_empty(linklist head)
{
    if (head == nullptr){   //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    if(head->next != nullptr){     //ֻ���ж���Ԫ�ڵ�
        return false;
    }
    return true;
}

//��ȡ������
int list_length(linklist head)
{
    if (head == nullptr){     //�������ڣ�����-1
        fprintf(stderr, "��������: listExit Erro\n"); 
        return -1;
    }
    linklist cur = head->next;
    int count = 0;
    while(cur)
    {
        count ++;
        cur = cur->next;   //ָ�����
    }
    return count;
}

//��ȡ��i���ڵ�
bool get_elem(linklist head, int i, elemtype &e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    linklist cur = head->next;
    int pos = 0;       //��¼λ��
    while(cur && pos < i)    //iԽ�����ѭ��
    {
        pos ++;
        if (pos == i){      //�ҵ���i���ڵ�
            e = cur->data;  //����ֵ��e��
            return true;
        }
        cur = cur->next;        //ָ�����
    }
    return false;
}

//�ҵ�������e��λ��
int locate_elem(linklist head, elemtype e, bool (*compare)(elemtype, elemtype))
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return -1;
    }
    linklist cur = head->next;
    int pos = 0;
    while(cur)
    {
        pos ++;
        if (!compare(e, cur->data)){   //�ҵ�����λ��
            return pos;
        }
        cur = cur->next;        //ָ�����
    }
    return -1;      //�Ҳ�������-1
}


//���ݽڵ������ҵ��ڵ�ǰ������
bool prior_elem(linklist head, elemtype cur_e, elemtype &pre_e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    linklist cur = head->next, pre = head;
    while(cur)
    {
        if(!datacmp(cur->data, cur_e)){     //�͵�ǰ�ڵ�Ƚ�
            pre_e = pre->data;
            return true;
        }
        pre = cur;      //�ڵ����
        cur = pre->next;
    }
    return false;
}

//���ݽڵ������ҵ���̽ڵ�����
bool next_elem(linklist head, elemtype cur_e, elemtype &next_e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    if(!head->next || !head->next->next){   //����Ϊ�ջ���ֻ��һ���ڵ�
        return false;
    }
    linklist cur = head->next;
    while(cur->next)
    {
        if (!datacmp(cur_e, cur->data)){    //����ҵ�
            next_e = cur->next->data;         //���غ�̽ڵ�����
            return true;
        }
        cur = cur->next;    //�ڵ����
    }
    return false;
}

//�ڵ�i���ڵ㴦����Ԫ��
bool list_insert(linklist &head, int i, elemtype e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    linklist prev = head;
    int pos = 0;
    while(prev && pos < i)
    {
        if (pos == i - 1){
            linklist newnode;
            create_node(newnode, e);   //��������ڵ�
            if(newnode == nullptr){
                fprintf(stderr, "�������ʧ��: listInsert Erro\n"); 
                break;
            }
            newnode->next = prev->next;         //����ڵ�
            prev->next = newnode;
            return true;
        }
        prev = prev->next;    //�ڵ����
        pos ++;
    }
    if(prev == nullptr){    //Խ��
        fprintf(stderr, "�±�Խ��: Index Erro\n"); 
    }
    return false;
}

//ɾ����i���ڵ�
bool list_delete(linklist &head, int i, elemtype &e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    linklist prev = head;    //��ֱ��ǰ��
    int pos = 0;
    while(prev->next)
    {
        if (pos == i - 1){
            linklist dst = prev->next;   //�����i���ڵ�
            prev->next = prev->next->next;  //����ָ��
            e = dst->data;
            delete dst;
            dst = nullptr;
            return true;
        }
        prev = prev->next;        //�ڵ����
        pos ++;
    }
    if(prev->next == nullptr){    //Խ��
        fprintf(stderr, "�±�Խ��: Index Erro\n");
    }
    return false;
}

//��ӡ����
void list_print(linklist head)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return ;
    }
    linklist cur = head->next;
    while(cur)
    {
        std::cout << cur->data << " ";   //��ӡ����
        cur = cur->next;        //�ڵ����
    }
    std::cout << std::endl;
}

/*
--------------------------------------------��������-----------------------------------------------------
*/

//���ش�start��ʼ��һ���ָ�����λ��
elemtype::size_type find_first_delimiter(elemtype str, elemtype::size_type size, elemtype::size_type start, int &language)
{
    std::string::size_type len = size - 1;
    std::string english_signs = " ),./;:'[]`\"<>?!(|";
    std::string chinese_signs = "������������������������������������";

    if(start == len){            //���start��str���һ���ֽ�
        for(int k = 0; k < 18; k++){     
            if (str[len] == english_signs[k]){
                language = ENGLISH;
                return len;
            }
        }
    }
    for(int i = start; i < len; i++){
        for (int j = 0; j < 35; j+=2){     //�ж����ı�����
            if ((str[i] == chinese_signs[j]) && (str[i + 1] == chinese_signs[j + 1])){
                language = CHINESE;
                return i;
            }
        }
        for(int k = 0; k < 19; k++){     //�ж�Ӣ�ı����źͿո�
            if (str[i] == english_signs[k]){
                language = ENGLISH;
                return i;
            }
        }
    }
    return size;
}


//�����ݷ���������
void create_list(linklist &head, elemtype str)
{
    if(head == nullptr){
        fprintf(stderr, "��������: listExit Erro\n");
        return ;
    }
    linklist cur = head;
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word; 
    int language = 1;     //�ָ������Ļ���Ӣ�ģ�������һ�β�������һ���ֽڻ�������
    if(size <= 0){      //�ַ���Ϊ�գ��ÿ�����
        clear_list(head);
        return ;
    }
    while(start <= (size - 1)){
        end = find_first_delimiter(str, size, start, language);
        if (end != start){
            word = str.substr(start, end - start);
            linklist newnode;           //���붯̬�ڴ棬�����½ڵ�
            create_node(newnode, word);
            if(word == "" || word == " "){
                start = start + language; 
                continue;
            }
            if(newnode == nullptr){            //����ʧ��
                fprintf(stderr, "������ʧ��: Createlist Error\n");
                destroy_list(head);
                return ;
            }
            //���½ڵ���뵽����ĩβ
            cur->next = newnode;
            cur = cur->next;   //�ڵ����
        }
        start = end + language;
    }
}

//���Ƿǿ�����
void cover_list(linklist &head, elemtype str)
{
    //ԭ�������ڻ��ߴ��ڵ�����Ϊ��
    if(head == nullptr || head->next == nullptr){ 
        fprintf(stderr, "��������: listExit Erro\n");  
        return ;
    }
    linklist cur = head->next;
    linklist phead = nullptr;  //�������һ�����ǽڵ�
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word; 
    int language = 0;

    if(size <= 0){      //�ַ���Ϊ�գ��ÿ�����
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
            cur = cur->next;   //�ڵ����
        }
        start = end + language; 
    }
    //�������ԭ���������ȣ�����
    while(cur){
        destroy_list(cur);
        phead->next = nullptr;
    }

    //���뵥��������������,����
    if(start < size){
        create_list(phead, str.substr(start));
    }
   
}


//�Ӽ���������������(β��)
void create_list_from_input(linklist &head)
{
    //�����ַ���
    std::string str;     
    if (std::getline(std::cin, str)) {  //��ȡ�ɹ�
        if (head != nullptr && head->next){   // ��������Ҳ�Ϊ�գ�ֱ�Ӹ���ԭ����
            cover_list(head, str);
            return ;
        }

        //�������ڣ�����������
        if(head == nullptr){
            create_node(head, "");  //ͷ�ڵ�
            if (head == nullptr){                      //�ж������Ƿ񴴽��ɹ�
                fprintf(stderr, "������ʧ��: Createlist Error\n");
                return ; 
            }    
        }
        create_list(head, str);   
    }
    // ��ȡʧ��
    else{
        fprintf(stderr, "����ʧ��: KeyboardInput error\n");
        return ;
    }
}

//���ļ�������������(β��)
void create_list_from_file(linklist &head, std::string path)
{
    std::ifstream file(path); // ���ļ����ڶ�ȡ
    if (!file.is_open()) {
        fprintf(stderr, "�ļ���ȡʧ��: FileOpen error\n");  // ����ļ����ܴ�
        return ;
    }
    //�����ַ���
    elemtype str;     
    if (std::getline(file, str)) {  //��ȡ�ɹ�
        if (head != nullptr && head->next){   // ��������Ҳ�Ϊ�գ�ֱ�Ӹ���ԭ����
            cover_list(head, str);
            return ;
        }

        //�������ڣ�����������
        if(head == nullptr){
            create_node(head, "");   //ͷ�ڵ�
            if (head == nullptr){                      //�ж������Ƿ񴴽��ɹ�
                fprintf(stderr, "������ʧ��: Createlist Error\n");
                return ; 
            }    
        }
        create_list(head, str);   
    }
    file.close();
}

//��ָ��λ�ò���һ����������
bool list_insert_words(linklist &head, int i, linklist words)
{
    if (head == nullptr || words == nullptr){
        fprintf(stderr, "��������: listExit Error\n"); 
        return false;
    }
    linklist prev = head;     //����λ��ǰһ���ڵ�
    int pos = 0;
    while(prev && pos < i - 1)    //�ҵ���i-1���ڵ�
    {
        prev = prev->next;    //�ڵ����
        pos ++;
    }
    if(prev == nullptr){    //Խ��
        fprintf(stderr, "�±�Խ��: Index Error\n"); 
        return false;
    }
    
    linklist r_tail = words->next;
    while(r_tail->next){
        r_tail = r_tail->next;
    }
    //���뵥��
    r_tail->next = prev->next;
    prev->next = words->next;

    return true;
}

//��ָ��λ��ɾ��һ����������
bool list_delete_words(linklist &head, int i, int n, linklist &result)
{   
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Error\n"); 
        return false;
    }
    linklist prev = head;    
    int pos =0;
    while(prev->next && pos < i - 1)  //��ֱ��ǰ��
    {
        prev = prev->next;        //�ڵ����
        pos ++;
    }
    linklist tmp;
    linklist r_cur = result;
    while(prev->next && n--){
        tmp = prev->next;     //����ָ��
        prev->next = prev->next->next;   //ɾ������
        r_cur->next = tmp;     //ɾ���ĵ��ʷŵ�result������
        r_cur = tmp;
    }
    r_cur->next = nullptr;   //��ֹ��ԭ�ַ������Ӱ��
    if(n > 0){
        fprintf(stderr, "Խ��: Index Error\n"); 
        return false;
    }
    return true;
}

//������
void list_reverse(linklist &head)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return ;
    }
    if (head->next == nullptr || head->next->next == nullptr){   //������򵥽ڵ�ֱ�ӷ���
        return ;
    } 
    linklist prev = nullptr;
    linklist cur = head->next;
    linklist Lnext = nullptr;     //���ú��ַ������׽ڵ�
    while(cur){
        Lnext = cur->next;
        cur->next = prev;   //�ı�ָ�뷽��
        prev = cur;     //�ڵ����
        cur = Lnext;
    }
    head->next = prev;  //ͷ���ָ���µ��׽ڵ�

}

//��ĳһ���Ӵ����ò����ص��ú�ĵ����ַ���
linklist list_range_reverse(linklist head, int start, int end)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return nullptr;
    }
    int pos = 1;
    linklist newhead;
    create_node(newhead, "");
    if(newhead == nullptr){
        fprintf(stderr, "����ʧ��: listReverse Erro\n"); 
        return nullptr;
    }

    newhead->next = nullptr;
    linklist cur = head->next;
    while(cur && pos < start){    //�ҵ���start���ڵ�
        pos ++;
        cur = cur->next;
    }
    while(cur && pos <= end && pos >= start)
    {
        list_insert(newhead, 1, cur->data);  //ͷ�壬�����Ӵ�
        cur = cur->next;        //ָ�����
        pos ++;
    }

    return newhead;
}

//�ж��Ƿ��ǻ��Ĵ�(�ʼ���)
bool is_palindrome(linklist head)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    int length = list_length(head);      //�ַ�������
    int start = length / 2 + 1;        //���ÿ�ʼλ��
    linklist R_head = list_range_reverse(head, start, length);        //�ַ������εĵ����ַ���
    linklist R_cur = R_head->next;
    linklist cur = head->next;
    for (int i = 1 ; i < start; i++){
        if (datacmp(cur->data, R_cur->data)){      //��������
            return false;
        }  
        cur = cur->next;
        R_cur = R_cur->next;    //ָ�����
    }
    return true;
}

//�����Ӵ�
bool search_sublist(linklist head, linklist pattern, linklist &result)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return false;
    }
    if (pattern == nullptr || pattern->next == nullptr){   //ƥ�䴮�����ڻ���Ϊ��
        return false;
    }
    linklist left = head->next, right = head->next;    //�ַ�������ƥ��ָ��
    linklist subright = pattern->next;   //ƥ�䴮��ǰƥ���λ��
    int pos = 1, count = 0;       //ƥ���λ�úʹ���
    while(right && left){
        while(right && subright){
            if (datacmp(right->data, subright->data)){  //����ȣ�head�ַ�������һ���ڵ㣬����ƥ���Ӵ�
                left = left->next;
                right = left;
                pos ++;
                subright = pattern->next;
                continue;
            }
            right = right->next;
            subright = subright->next;      //�ڵ����
            }
        if (!subright){     //�Ӵ�ƥ�����
            count++;
            list_insert(result, count, std::to_string(pos));   //��¼λ��
            subright = pattern->next;
        }

        if(!left){     //����ƥ����
           break;
        }
        left = left->next;     //�������Ƽ���ƥ��
        right = left;
        pos ++;
            
    }
    if(count > 0){    //����ƥ��һ��
        return true;
    }
    return false;
}

//head2�ϲ���head1�����ַǵݼ�
void merge_list(linklist &head1, linklist head2)
{
    if (head1 == nullptr || head2 == nullptr){      //����1��2������
        fprintf(stderr, "��������: listExit Erro\n"); 
        return ;
    }

    linklist next1 = head1->next;    //����1β���
    linklist next2 = head2->next;     //����2β���
    linklist cur = head1;               //������ͷ���
    while(next1 && next2){
        if(datacmp(next1->data, next2->data) <= 0){      //tail1С�ڵ���tail2, ����1�ڵ����������
            cur->next = next1;
            cur = next1;
            next1 = next1->next;
        }
        else{                               //tail2С��tail1,����2�ڵ����������
            cur->next = next2;
            cur = next2;
            next2 = next2->next;
        }
    }
    if(next1){      //���head1����δ����
        cur->next = next1;
    }
    if (next2){      //���head2����δ����
        cur->next = next2;
    }
}



/*

//�����ǵݼ����кϲ���һ���µķǵݼ�����,���ͷ�ԭ����������ռ�,�ϲ���ֻ��Ҫ�ͷ����������ͷ���
void NewMergelist(linklist &head1, linklist head2)
{
    
    if(head1 == nullptr || head2 == nullptr){
        fprintf(stderr, "��������: listExit Error\n"); 
        return nullptr;
    }

    linklist Mergehead;
    CreateNode(Mergehead, "");
    if (Mergehead == nullptr){
        fprintf(stderr, "�ϲ�ʧ��: Mergelist Error\n"); 
        return nullptr;
    }
    Mergehead->next = nullptr;
    
    //����1/2������
    linklist next1 = head1->next;    //����1β���
    linklist next2 = head2->next;
    linklist cur = Mergehead;
    while(next1 && next2)
    {
        if(compare(next1->data, next2->data) <= 0){      //tail1С�ڵ���tail2,����1�ڵ����������
            cur->next = next1;
            cur = next1;
            next1 = next1->next;
        }
        else{                               //tail2С��tail1,����2�ڵ����������
            cur->next = next2;
            cur = next2;
            next2 = next2->next;
        }
    }
    if(next1){      //���head1����δ����
        cur->next = next1;
    }
    if (next2){      //���head2����δ����
        cur->next = next2;
    }
}
*/


//�����ǵݼ����кϲ���һ���µķǵݼ�����,�ϲ������ͷ���������ռ�
linklist new_merge_list(linklist head1, linklist head2)
{   
    if(head1 == nullptr || head2 == nullptr){       //�ϲ������ڵ�����Ƿ�
        fprintf(stderr, "��������: listExit Error\n"); 
        return nullptr;
    }

    linklist mergehead;     //����ʧ��
    create_node(mergehead, "");
    if (mergehead == nullptr){
        fprintf(stderr, "�ϲ�ʧ��: Mergelist Error\n"); 
        return nullptr;
    }
    
    linklist next1 = head1->next;    //����1β���
    linklist next2 = head2->next;
    linklist cur = mergehead;
    int pos = 1;   //�����λ��
    while(next1 && next2)
    {
        if(datacmp(next1->data, next2->data) <= 0){      //tail1С�ڵ���tail2,,����1�ڵ����������
            linklist newnode;
            create_node(newnode, next1->data);
            if (newnode == nullptr){
                fprintf(stderr, "�ϲ�ʧ��: Mergelist Error\n"); 
                destroy_list(mergehead);    //���٣������ڴ�й¶
                return nullptr;
            }
            cur->next = newnode;    //�½ڵ����ϲ�����
            cur = cur->next;        //����ָ��
            next1 = next1->next;
        }
        else{                               //tail2С��tail1,����2�ڵ����������
            linklist newnode;
            create_node(newnode, next2->data);
            if (newnode == nullptr){
                fprintf(stderr, "�ϲ�ʧ��: Mergelist Error\n"); 
                destroy_list(mergehead);    //���٣������ڴ�й¶
                return nullptr;
            }

            cur->next = newnode;    //�½ڵ����ϲ�����
            cur = cur->next;        //����ָ��
            next2 = next2->next;
        }
    }
    while(next1){      //���head1����δ����
         linklist newnode;
        create_node(newnode, next1->data);
        if (newnode == nullptr){
            fprintf(stderr, "�ϲ�ʧ��: Mergelist Error\n"); 
            destroy_list(mergehead);    //���٣������ڴ�й¶
            return nullptr;
        }

        cur->next = newnode;    //�½ڵ����ϲ�����
        cur = cur->next;        //����ָ��
        next1 = next1->next;
    }
    while(next2){      //���head2����δ����
        linklist newnode;
        create_node(newnode, next2->data);
        if (newnode == nullptr){
            fprintf(stderr, "�ϲ�ʧ��: Mergelist Error\n"); 
            destroy_list(mergehead);    //���٣������ڴ�й¶
            return nullptr;
        }

        cur->next = newnode;    //�½ڵ����ϲ�����
        cur = cur->next;        //����ָ��
        next2 = next2->next;
    }
    return mergehead;
}
