#include"LinkList.h"
#define CHINESE 2
#define ENGLISH 1

//�Ƚ����ݴ�С
int datacmp(ElemType e1, ElemType e2)
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
void nodecpy(LinkList &dst, LinkList src)
{
    if (src == nullptr || dst == nullptr){
        fprintf(stderr, "����ʧ��: NodeCopy Error\n"); 
        return ;
    }
    dst->data = src->data;
}

//�����ڵ�
void create_node(LinkList &node, ElemType e)
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

//���ش�start��ʼ��һ���ָ�����λ��
ElemType::size_type find_first_delimiter(ElemType str, ElemType::size_type size, ElemType::size_type start, int &language)
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
void create_List(LinkList &head, ElemType str)
{
    if(head == nullptr){
        fprintf(stderr, "��������: ListExit Erro\n");
        return ;
    }
    LinkList cur = head;
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word; 
    int language = 1;     //�ָ������Ļ���Ӣ�ģ�������һ�β�������һ���ֽڻ�������
    if(size <= 0){      //�ַ���Ϊ�գ��ÿ�����
        clear_List(head);
        return ;
    }
    while(start <= (size - 1)){
        end = find_first_delimiter(str, size, start, language);
        if (end != start){
            word = str.substr(start, end - start);
            LinkList newnode;           //���붯̬�ڴ棬�����½ڵ�
            create_node(newnode, word);
            if(word == "" || word == " "){
                start = start + language; 
                continue;
            }
            if(newnode == nullptr){            //����ʧ��
                fprintf(stderr, "������ʧ��: CreateList Error\n");
                destroy_List(head);
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
void cover_List(LinkList &head, ElemType str)
{
    //ԭ�������ڻ��ߴ��ڵ�����Ϊ��
    if(head == nullptr || head->next == nullptr){ 
        fprintf(stderr, "��������: ListExit Erro\n");  
        return ;
    }
    LinkList cur = head->next;
    LinkList phead = nullptr;  //�������һ�����ǽڵ�
    std::string::size_type end = 0, start = 0, size = str.length();
    std::string word; 
    int language = 0;

    if(size <= 0){      //�ַ���Ϊ�գ��ÿ�����
        clear_List(head);
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
        destroy_List(cur);
        phead->next = nullptr;
    }

    //���뵥��������������,����
    if(start < size){
        create_List(phead, str.substr(start));
    }
   
}


//�Ӽ���������������(β��)
void create_List_from_input(LinkList &head)
{
    //�����ַ���
    std::string str;     
    if (std::getline(std::cin, str)) {  //��ȡ�ɹ�
        if (head != nullptr && head->next){   // ��������Ҳ�Ϊ�գ�ֱ�Ӹ���ԭ����
            cover_List(head, str);
            return ;
        }

        //�������ڣ�����������
        if(head == nullptr){
            create_node(head, "");  //ͷ�ڵ�
            if (head == nullptr){                      //�ж������Ƿ񴴽��ɹ�
                fprintf(stderr, "������ʧ��: CreateList Error\n");
                return ; 
            }    
        }
        create_List(head, str);   
    }
    // ��ȡʧ��
    else{
        fprintf(stderr, "����ʧ��: KeyboardInput error\n");
        return ;
    }
}

//���ļ�������������(β��)
void create_List_from_file(LinkList &head, std::string path)
{
    std::ifstream file(path); // ���ļ����ڶ�ȡ
    if (!file.is_open()) {
        fprintf(stderr, "�ļ���ȡʧ��: FileOpen error\n");  // ����ļ����ܴ�
        return ;
    }
    //�����ַ���
    ElemType str;     
    if (std::getline(file, str)) {  //��ȡ�ɹ�
        if (head != nullptr && head->next){   // ��������Ҳ�Ϊ�գ�ֱ�Ӹ���ԭ����
            cover_List(head, str);
            return ;
        }

        //�������ڣ�����������
        if(head == nullptr){
            create_node(head, "");   //ͷ�ڵ�
            if (head == nullptr){                      //�ж������Ƿ񴴽��ɹ�
                fprintf(stderr, "������ʧ��: CreateList Error\n");
                return ; 
            }    
        }
        create_List(head, str);   
    }
    file.close();
}

//��������
void destroy_List(LinkList &head)
{
    if (head == nullptr){    //��������
        // fprintf(stderr, "��������: ListExit Erro\n"); 
        return ;
    }
    LinkList next = nullptr, cur = head->next;
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
void clear_List(LinkList &head)
{
    if (head == nullptr){
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return ;
    }
    LinkList next = nullptr, cur = head->next;
    while(cur)
    {
        next = cur->next;
        delete cur;          //�ͷŽڵ�
        cur = nullptr;
        cur = next;       //�ڵ����
    }
    head->next = nullptr;
}

//�ж������Ƿ�Ϊ��
bool List_empty(LinkList head)
{
    if (head == nullptr){   //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    if(head->next != nullptr){     //ֻ���ж�ͷ�����һ���ڵ��Ƿ����
        return true;
    }
    return false;
}

//��ȡ������
int List_length(LinkList head)
{
    if (head == nullptr){     //�������ڣ�����-1
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return -1;
    }
    LinkList cur = head->next;
    int count = 0;
    while(cur)
    {
        count ++;
        cur = cur->next;   //ָ�����
    }
    return count;
}

//��ȡ��i���ڵ�
bool get_elem(LinkList head, int i, ElemType &e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    LinkList cur = head->next;
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
int locate_elem(LinkList head, ElemType e, DataCompare compare)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return -1;
    }
    LinkList cur = head->next;
    int pos = 0;
    while(cur)
    {
        pos ++;
        if (!datacmp(e, cur->data)){   //�ҵ�����λ��
            return pos;
        }
        cur = cur->next;        //ָ�����
    }
    return -1;      //�Ҳ�������-1
}


//���ݽڵ������ҵ��ڵ�ǰ������
bool prior_elem(LinkList head, ElemType cur_e, ElemType &pre_e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    LinkList cur = head->next, pre = head;
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
bool next_elem(LinkList head, ElemType cur_e, ElemType &next_e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    LinkList cur = head->next;
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
bool List_insert(LinkList &head, int i, ElemType e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = head;
    int pos = 0;
    while(prev && pos < i)
    {
        if (pos == i - 1){
            LinkList newnode;
            create_node(newnode, e);   //��������ڵ�
            if(newnode == nullptr){
                fprintf(stderr, "�������ʧ��: ListInsert Erro\n"); 
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


//��ָ��λ�ò���һ����������
bool List_insert_words(LinkList &head, int i, LinkList result)
{
    if (head == nullptr || result == nullptr){
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = head;
    int pos = 1;
    while(prev && pos < i - 1)    //�ҵ���i-1���ڵ�
    {
        prev = prev->next;    //�ڵ����
        pos ++;
    }
    if(prev == nullptr){    //Խ��
        fprintf(stderr, "�±�Խ��: Index Erro\n"); 
        return false;
    }
    
    LinkList r_tail = result->next;
    while(r_tail->next){
        r_tail = r_tail->next;
    }
    //���뵥��
    r_tail->next = prev->next;
    prev->next = r_tail;

    return true;
}

//ɾ����i���ڵ�
bool List_delete(LinkList &head, int i, ElemType &e)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = head;    //��ֱ��ǰ��
    int pos = 0;
    while(prev->next)
    {
        if (pos == i - 1){
            LinkList dst = prev->next;   //�����i���ڵ�
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


//��ָ��λ��ɾ��һ����������
bool List_delete_words(LinkList &head, int i, int n, LinkList &result)
{   
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    LinkList prev = head;    
    int pos = 1;
    while(prev->next && pos < i - 1)  //��ֱ��ǰ��
    {
        prev = prev->next;        //�ڵ����
        pos ++;
    }
    LinkList tmp = nullptr;
    LinkList r_cur = result;
    while(prev->next && --n){
        tmp = prev->next;     //����ָ��
        prev->next = prev->next->next;   //ɾ������
        r_cur->next = tmp;     //���ʷŵ�result������
        r_cur = tmp;
    }
    return true;
}

//��ӡ����
void List_print(LinkList head)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return ;
    }
    LinkList cur = head->next;
    while(cur)
    {
        std::cout << cur->data << " ";   //��ӡ����
        cur = cur->next;        //�ڵ����
    }
    std::cout << std::endl;
}

//������
void List_reverse(LinkList &head)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return ;
    }
    if (head->next == nullptr || head->next->next == nullptr){   //������򵥽ڵ�ֱ�ӷ���
        return ;
    } 
    LinkList prev = nullptr;
    LinkList cur = head->next;
    LinkList Lnext = nullptr;
    while(cur){
        Lnext = cur->next;
        cur->next = prev;   //�ı�ָ�뷽��
        prev = cur;     //�ڵ����
        cur = Lnext;
    }
    head->next = prev;  //ͷ���ָ���µ��׽ڵ�

}

//��ĳһ���Ӵ����ò����ص��ú�ĵ����ַ���
LinkList List_range_reverse(LinkList head, int start, int end)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return nullptr;
    }
    int pos = 1;
    LinkList newhead;
    create_node(newhead, "");
    if(newhead == nullptr){
        fprintf(stderr, "����ʧ��: ListReverse Erro\n"); 
        return nullptr;
    }

    newhead->next = nullptr;
    LinkList cur = head->next;
    while(cur && pos < start){    //�ҵ���start���ڵ�
        pos ++;
        cur = cur->next;
    }
    while(cur && pos <= end && pos >= start)
    {
        List_insert(newhead, 1, cur->data);  //ͷ�壬�����Ӵ�
        cur = cur->next;        //ָ�����
        pos ++;
    }

    return newhead;
}

//�ж��Ƿ��ǻ��Ĵ�(�ʼ���)
bool is_palindrome(LinkList head)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    int length = List_length(head);      //�ַ�������
    int start = length / 2 + 1;        //���ÿ�ʼλ��
    LinkList R_head = List_range_reverse(head, start, length);        //�ַ������εĵ����ַ���
    LinkList R_cur = R_head->next;
    LinkList cur = head->next;
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
bool search_subList(LinkList head, LinkList pattern, LinkList &result)
{
    if (head == nullptr){      //��������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return false;
    }
    if (pattern == nullptr || pattern->next == nullptr){   //ƥ�䴮�����ڻ���Ϊ��
        return false;
    }
    LinkList left = head->next, right = head->next;    //�ַ�������ƥ��ָ��
    LinkList subright = pattern->next;   //ƥ�䴮��ǰƥ���λ��
    int pos = 1, count = 0;
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
            List_insert(result, count, std::to_string(pos));
            subright = pattern->next;
        }

        if(!left){     //����ƥ����
           break;
        }
        left = left->next;
        right = left;
        pos ++;
            
    }
    if(count > 0){
        return true;
    }
    return false;
}

//head2�ϲ���head1�����ַǵݼ�
void merge_List(LinkList &head1, LinkList head2)
{
    if (head1 == nullptr || head2 == nullptr){      //����1��2������
        fprintf(stderr, "��������: ListExit Erro\n"); 
        return ;
    }

    LinkList next1 = head1->next;    //����1β���
    LinkList next2 = head2->next;     //����2β���
    LinkList cur = head1;               //������ͷ���
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
void NewMergeList(LinkList &head1, LinkList head2)
{
    
    if(head1 == nullptr || head2 == nullptr){
        fprintf(stderr, "��������: ListExit Error\n"); 
        return nullptr;
    }

    LinkList Mergehead;
    CreateNode(Mergehead, "");
    if (Mergehead == nullptr){
        fprintf(stderr, "�ϲ�ʧ��: MergeList Error\n"); 
        return nullptr;
    }
    Mergehead->next = nullptr;
    
    //����1/2������
    LinkList next1 = head1->next;    //����1β���
    LinkList next2 = head2->next;
    LinkList cur = Mergehead;
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
LinkList new_merge_List(LinkList head1, LinkList head2)
{   
    if(head1 == nullptr || head2 == nullptr){       //�ϲ������ڵ�����Ƿ�
        fprintf(stderr, "��������: ListExit Error\n"); 
        return nullptr;
    }

    LinkList mergehead;     //����ʧ��
    create_node(mergehead, "");
    if (mergehead == nullptr){
        fprintf(stderr, "�ϲ�ʧ��: MergeList Error\n"); 
        return nullptr;
    }
    
    LinkList next1 = head1->next;    //����1β���
    LinkList next2 = head2->next;
    LinkList cur = mergehead;
    int pos = 1;   //�����λ��
    while(next1 && next2)
    {
        if(datacmp(next1->data, next2->data) <= 0){      //tail1С�ڵ���tail2,,����1�ڵ����������
            LinkList newnode;
            create_node(newnode, next1->data);
            if (newnode == nullptr){
                fprintf(stderr, "�ϲ�ʧ��: MergeList Error\n"); 
                destroy_List(mergehead);    //���٣������ڴ�й¶
                return nullptr;
            }
            cur->next = newnode;    //�½ڵ����ϲ�����
            cur = cur->next;        //����ָ��
            next1 = next1->next;
        }
        else{                               //tail2С��tail1,����2�ڵ����������
            LinkList newnode;
            create_node(newnode, next2->data);
            if (newnode == nullptr){
                fprintf(stderr, "�ϲ�ʧ��: MergeList Error\n"); 
                destroy_List(mergehead);    //���٣������ڴ�й¶
                return nullptr;
            }

            cur->next = newnode;    //�½ڵ����ϲ�����
            cur = cur->next;        //����ָ��
            next2 = next2->next;
        }
    }
    while(next1){      //���head1����δ����
         LinkList newnode;
        create_node(newnode, next1->data);
        if (newnode == nullptr){
            fprintf(stderr, "�ϲ�ʧ��: MergeList Error\n"); 
            destroy_List(mergehead);    //���٣������ڴ�й¶
            return nullptr;
        }

        cur->next = newnode;    //�½ڵ����ϲ�����
        cur = cur->next;        //����ָ��
        next1 = next1->next;
    }
    while(next2){      //���head2����δ����
        LinkList newnode;
        create_node(newnode, next2->data);
        if (newnode == nullptr){
            fprintf(stderr, "�ϲ�ʧ��: MergeList Error\n"); 
            destroy_List(mergehead);    //���٣������ڴ�й¶
            return nullptr;
        }

        cur->next = newnode;    //�½ڵ����ϲ�����
        cur = cur->next;        //����ָ��
        next2 = next2->next;
    }
    return mergehead;
}

//Ѱ����������Ĳ�ͬ�Ӵ�
LinkList SearchDiff(LinkList head1, LinkList head2)
{
    if (head1 == nullptr){
        std::cout << "����1������" << std::endl;
        return nullptr;
    }
    if (head2 == nullptr){
        std::cout << "����2������" << std::endl;
        return nullptr;
    }
    LinkList diff = nullptr;
    return diff;
}