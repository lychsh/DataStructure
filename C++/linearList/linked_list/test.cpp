#include"linklist.h"

void swapvalues(linklist &x, linklist &y)
{
    if(x == y){
        return ;
    }
    elemtype tmp = x->data;
    x->data = y->data;
    y->data = x->data;
}

linklist partition(linklist head, linklist tail) {
    // 选择最后一个元素作为基准
    elemtype pivot = tail->data;
    linklist post = nullptr; // post 指向小于基准的最后一个节点
    linklist cur = head;    // cur 用于遍历链表

    for (; cur != tail; cur = cur->next) {
        if (datacmp(cur->data, pivot) < 0) {
            if (post == nullptr) {
                post = head;
            } else {
                post = post->next;
            }
            swapvalues(post, cur);
        }
    }
    // 将基准值交换到正确的位置
    if (post != nullptr) {
        swapvalues(post->next, tail);
        return post->next;
    } else {
        swapvalues(head, tail);
        return head;
    }
}

void quick_sort(linklist head, linklist tail) {
    if (head == nullptr || head == tail || head->next == tail) {     //空指针，空链表或者只有一个节点返回
        return;
    }
    linklist p = partition(head, tail);
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


// int main()
// {
//     linklist head = nullptr;
//     create_list_from_input(head);
//     linklist src_head = head;      //保存头结点
//     head = head->next;            //变为没有头结点的链表              
//     linklist tail = head->next;
//     while(tail->next)    //找到尾指针
//     {
//         tail = tail->next;
//     }
//     quick_sort(head->next, tail);
//     src_head->next = head;
//     std::cout << "排序后为: " << std::endl;
//     list_print(head);

//     //-------------------------------------删除最小值-------------------------------------------------
//     // deletemin(head);
//     // std::cout << "删除最小值后链表为：" << std::endl;
//     // list_print(head);
//     return 0;
// }