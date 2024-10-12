#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<stdbool.h>
#include<iostream>
#include<sstream>
#include <fstream>

typedef std::string elemtype;  //节点数据类型

typedef struct LNode{    //链表节点
    elemtype data;         // 数据域
    LNode* next;            // 指针域
}LNode, *linklist;


/*
----------------------------------------------基本操作-----------------------------------------------------
*/

//复制节点
void nodecpy(linklist &dst, linklist src);

//比较数据大小
int datacmp(elemtype e1, elemtype e2);

//创建节点
void create_node(linklist &node, elemtype e);

//销毁链表
void destroy_list(linklist &head);

//清空链表，保留头指针
void clear_list(linklist &head);

//判断链表是否为空
bool list_empty(linklist head);

//获取链表长度
int list_length(linklist head);

//获取第i个节点
bool get_elem(linklist head, int i, elemtype &e);

//找到数据项e的位置
int locate_elem(linklist head, elemtype e, bool (*compare)(elemtype, elemtype));

//根据节点数据找到节点前驱数据
bool prior_elem(linklist head, elemtype cur_e, elemtype &pre_e);

//根据节点数据找到后继节点数据
bool next_elem(linklist head, elemtype cur_e, elemtype &next_e);

//在第i个节点处插入元素
bool list_insert(linklist &head, int i, elemtype e);

//删除第i个节点
bool list_delete(linklist &head, int i, elemtype &e);

//打印链表
void list_print(linklist head);


/*
--------------------------------------------初级功能-----------------------------------------------------
*/

//返回从start开始第一个分隔符的位置
elemtype::size_type find_first_delimiter(elemtype str, elemtype::size_type size, elemtype::size_type start, int &language);

//将数据放入链表中
void create_list(linklist &head, elemtype str);

//覆盖非空链表
void cover_list(linklist &head, elemtype str);

//从键盘输入链表内容(尾插)
void create_list_from_input(linklist &head);

//从文件读入链表内容(尾插)
void create_list_from_file(linklist &head, std::string path);

//在指定位置插入一个或多个单词
bool list_insert_words(linklist &head, int i, linklist result);

//在指定位置删除一个或多个单词
bool list_delete_words(linklist &head, int i, int n, linklist &result);

//链表倒置
void list_reverse(linklist &head);

//将某一段子串倒置并返回倒置后的的新字符串
linklist list_range_reverse(linklist head, int start, int end);

//判断是否是回文串(词级别)
bool is_palindrome(linklist head);

//查找子串
bool search_sublist(linklist head, linklist pattern, linklist &result);

//两个非递减序列合并成一个非递减序列
linklist new_merge_list(linklist head1, linklist head2);

//两个非递减序列合并成一个新的非递减序列,不释放原来两个链表空间,合并后只需要释放两个链表的头结点
//void NewMergelist(linklist &head1, linklist head2)

//head2合并到head1，保持非递减
void merge_list(linklist &head1, linklist head2);

