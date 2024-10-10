#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<stdbool.h>
#include<iostream>
#include<sstream>
#include <fstream>

typedef std::string ElemType;  //节点数据类型

typedef struct LNode{    //链表节点
    ElemType data;         // 数据域
    LNode* next;            // 指针域
}LNode, *LinkList;

//数据比较函数
typedef int (*DataCompare)(ElemType, ElemType);

//复制节点
void nodecpy(LinkList &dst, LinkList src);

//比较数据大小
int datacmp(ElemType e1, ElemType e2);

//创建节点
void create_node(LinkList &node, ElemType e);

//返回从start开始第一个分隔符的位置
ElemType::size_type find_first_delimiter(ElemType str, ElemType::size_type size, ElemType::size_type start, int &language);

//将数据放入链表中
void create_List(LinkList &head, ElemType str);

//覆盖非空链表
void cover_List(LinkList &head, ElemType str);

//从键盘输入链表内容(尾插)
void create_List_from_input(LinkList &head);

//从文件读入链表内容(尾插)
void create_List_from_file(LinkList &head, std::string path);

//销毁链表
void destroy_List(LinkList &head);

//清空链表，保留头指针
void clear_List(LinkList &head);

//判断链表是否为空
bool List_empty(LinkList head);

//获取链表长度
int List_length(LinkList head);

//获取第i个节点
bool get_elem(LinkList head, int i, ElemType &e);

//找到数据项e的位置
int locate_elem(LinkList head, ElemType e, DataCompare compare);


//根据节点数据找到节点前驱数据
bool prior_elem(LinkList head, ElemType cur_e, ElemType &pre_e);

//根据节点数据找到后继节点数据
bool next_elem(LinkList head, ElemType cur_e, ElemType &next_e);

//在第i个节点处插入元素
bool List_insert(LinkList &head, int i, ElemType e);

//在指定位置插入一个或多个单词
bool List_insert_words(LinkList &head, int i, LinkList result);

//删除第i个节点
bool List_delete(LinkList &head, int i, ElemType &e);

//在指定位置删除一个或多个单词
bool List_delete_words(LinkList &head, int i, int n, LinkList &result);

//打印链表
void List_print(LinkList head);

//链表倒置
void List_reverse(LinkList &head);

//将某一段子串倒置并返回倒置后的的新字符串
LinkList List_range_reverse(LinkList head, int start, int end);

//判断是否是回文串(词级别)
bool is_palindrome(LinkList head);

//查找子串
bool search_subList(LinkList head, LinkList pattern, LinkList &result);

//两个非递减序列合并成一个非递减序列
LinkList new_merge_List(LinkList head1, LinkList head2);

//两个非递减序列合并成一个新的非递减序列,不释放原来两个链表空间,合并后只需要释放两个链表的头结点
//void NewMergeList(LinkList &head1, LinkList head2)

//head2合并到head1，保持非递减
void merge_List(LinkList &head1, LinkList head2);

//寻找两个链表的不同子串
LinkList search_diff(LinkList head1, LinkList head2);
