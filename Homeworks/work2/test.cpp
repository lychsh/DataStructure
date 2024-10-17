#include"linklist.h"

void swapvalues(linklist &x, linklist &y)
{
    if(x == y){
        return ;
    }
    elemtype tmp = x->data;
    x->data = y->data;
    y->data = tmp;
}

linklist partition(linklist &head, linklist &tail) {
    // 选择最后一个元素作为基准
    elemtype pivot = tail->data;
    linklist post = nullptr; // post 指向小于基准的最后一个节点
    linklist cur = head;    // cur 用于遍历链表

    for (; cur != tail; cur = cur->next) {
        if (datacmp(cur->data, pivot) < 0) {
            if (post == nullptr) {
                post = head;
            } 
            else {
                post = post->next;
            }
            if(post != cur){
                swapvalues(post, cur);
            }
        }
    }
    // 将基准值交换到正确的位置
    if (post != nullptr) {
        if(post->next == tail){    //前面的值都比tail小
            return post;
        }
        swapvalues(post->next, tail);
        return post->next;
    } 
    else{
        swapvalues(head, tail);
        return head;
    }
}

void quick_sort(linklist &head, linklist &tail) {
    if (head == nullptr || head == tail) {     //空指针，空链表或者只有一个节点返回
        return;
    }
    linklist p = partition(head, tail);
    if(p == tail){      //前面的值都比tail小
        return ;
    }
    quick_sort(head, p); // 排序基准左侧的元素
    quick_sort(p->next, tail); // 排序基准右侧的元素
}


void deletemin(linklist &head)
{
    if(!head){
        std::cout << "链表不存在" << std::endl;
        return ;
    }
    if(!head->next){
        std::cout << "链表为空" << std::endl;
        return ;
    }
    if(!head->next->next){   //只有一个元素
        delete head->next;
        head->next = nullptr;
        return ;
    }
    //至少有两个元素
    elemtype mindata = head->next->data;   //最小值
    linklist min_pre = head, pre = head;
    while(pre->next){
        if(datacmp(pre->next->data, min_pre->next->data) < 0){
            min_pre = pre;                //更新最小值前驱和最小值
            mindata = pre->next->data;
        }
        pre = pre->next;
    }
    linklist minnode = min_pre->next;    //保存最小值节点
    min_pre->next = min_pre->next->next;
    delete minnode;      //删除最小值节点
}



void listsort(linklist &head)
{
    if(!head){
        std::cout << "链表不存在" << std::endl;
        return ;
    }
    if(!head->next){   //只有一个元素
        return ;
    }
    linklist cur= head, pre_j, j, pre_k = nullptr, k, pre_cur = nullptr;
    while(cur->next){
        j = cur;
        k = cur; 
        while(j){              //找到比cur大的最大节点k
            if(datacmp(j->data, k->data) > 0){
                pre_k = pre_j;
                k = j;
            }
            pre_j = j;
            j = j->next;
        }
        if(k == cur){    //cur就是当前最大节点
            if(cur != head){     //cur不是头结点时
                pre_cur->next = pre_cur->next->next;    //移除节点cur
                cur->next = head;                        //头插cur
                head = cur;
                cur = pre_cur->next;                          //节点后移
            }
            else{
                pre_cur = cur;
                cur = cur->next;
            }
        }
        else{         //cur不是当前最大节点，k是当前最大节点
            k = pre_k->next;                        
            pre_k->next = pre_k->next->next;    //移除节点k
            k->next = head;                     //头插节点k
            head = k;                           //更新头节点
            if(!pre_cur){
                pre_cur = head;
            }
        }
    }
    pre_cur->next = nullptr;
    cur->next = head;
    head = cur;
}


int main()
{
    linklist head = nullptr;
    create_list_from_input(head);
    // linklist src_head = head;
    // head = head->next;
    // listsort(head);
    // src_head->next = head;
    // list_print(src_head);
    // return 0;
    //-------------------------------------删除最小值-------------------------------------------------
    deletemin(head);
    std::cout << "删除最小值后链表为：" << std::endl;
    list_print(head);
    return 0;
}