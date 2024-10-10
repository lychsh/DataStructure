#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include<assert.h>
#include<stdbool.h>
#include<iostream>
#define BUFFER_SIZE 1024
using namespace std;

typedef char* ElemType;  //节点数据类型

typedef struct LNode{    //链表节点
    ElemType data;         // 数据域
    LNode* next;            // 指针域
}LNode, *LinkList;

//数据比较函数
typedef int (*DataCompare)(ElemType, ElemType);

//求数据长度
int datalen(ElemType e);

//复制节点数据
void datacpy(ElemType &dst, ElemType src);

//复制节点
void Nodecpy(LinkList &dst, LinkList src);

//比较ElemType大小
int strcompare(ElemType e1, ElemType e2);

//创建节点
void CreateNode(LinkList &node, size_t size);

//将数据放入链表中
void CreateList(LinkList &Head, ElemType str);

//覆盖非空链表
void CoverList(LinkList &Head, ElemType str);


//从键盘输入链表内容(尾插)
void CreateListFromInput(LinkList &Head);

//从文件读入链表内容(尾插)
void CreateListFromFile(LinkList &Head, const char* path);

//销毁链表
void DestroyList(LinkList &Head);

//清空链表，保留头指针
void ClearList(LinkList &Head);

//判断链表是否为空
bool ListEmpty(LinkList Head);

//获取链表长度
int ListLength(LinkList Head);

//获取第i个节点
bool GetElem(LinkList Head, int i, ElemType &e);

//找到数据项e的位置
int LocateElem(LinkList Head, ElemType e, DataCompare compare);


//根据节点数据找到节点前驱数据
bool PriorElem(LinkList Head, ElemType cur_e, ElemType &pre_e);

//根据节点数据找到后继节点数据
bool NextElem(LinkList Head, ElemType cur_e, ElemType &next_e);

//在第i个节点处插入元素
bool ListInsert(LinkList &Head, int i, ElemType e);

//在指定位置插入一个或多个单词
bool ListInsertWords(LinkList &Head, int i, LinkList e);

//删除第i个节点
bool ListDelete(LinkList &Head, int i, ElemType &e);

//在指定位置删除一个或多个单词
bool ListDeleteWords(LinkList &Head, int i, int n, LinkList &result);

//打印链表
void ListPrint(LinkList Head);

//链表倒置
void ListReverse(LinkList &Head);

//将某一段子串倒置并返回倒置后的的新字符串
LinkList ListRangeReverse(LinkList Head, int start, int end);

//判断是否是回文串(词级别)
bool IsPalindrome(LinkList Head);

//查找子串
bool SearchSubList(LinkList Head, LinkList pattern);

//两个非递减序列合并成一个非递减序列
LinkList NewMergeList(LinkList Head1, LinkList Head2);

//两个非递减序列合并成一个新的非递减序列,不释放原来两个链表空间,合并后只需要释放两个链表的头结点
//void NewMergeList(LinkList &Head1, LinkList Head2)

//Head2合并到Head1，保持非递减
void MergeList(LinkList &Head1, LinkList Head2);

//寻找两个链表的不同子串
LinkList SearchDiff(LinkList Head1, LinkList Head2);
