#include<stdbool.h>
#include<stdlib.h>
#include<memory.h>
#include<assert.h>
#include<string.h>
#include<iostream>
using namespace std;

/*扩容策略
初始阶段，给定1000，避免多次扩容
倍数扩容阶段，两倍扩展，避免反复扩容
固定扩容阶段，容量较大时，避免扩容过大，空间利用率低
*/
#define LIST_INIT_SIZE 128  //线性表存储空间的初始分配量
#define MAX_DOUBLE_INCREMENT 2048    //按二倍扩容策略最大分配量
#define MULTIPLIER_FACTOR 2  //倍增因子
#define MAX_INCREMENT  100000 //固定扩容最大分配量
#define FIXED_INCREMENT  1000//固定增长分配量
#define BUFFER_SIZE 1024   //


typedef struct{
    char* str;     // 存放单词
}ElemType;

typedef struct{
    ElemType * elem;   //顺序表
    int length;     //表长(现有元素个数)，初始为0
    int listsize;   //存储容量，能容纳的元素个数
}SqList;

typedef bool (*Compare)(ElemType, ElemType);

//数据比较函数
bool strcompare(ElemType e1, ElemType e2);

//表满扩容
void CapacityExpansion(SqList &L);

//空间利用率过低，缩容
void CapacityReduce(SqList &L);

//顺序表初始化
bool InitList(SqList &L);

//覆盖原字符串
void CoverList(SqList &L);

//字符串输入
void ListInput(SqList &L);

//打印顺序表
void ListPrint(SqList L);

//顺序表销毁
bool DestroyList(SqList &L);

//清空顺序表
void ClearList(SqList &L);

//判断顺序表是否为空
bool ListEmpty(SqList L);

//获取顺序表长度
int ListLength(SqList L);

//获取第i个元素
bool GetElem(SqList L, int i, ElemType &e);

//查找元素e的位置
int LocateElem(SqList L, ElemType e, Compare compare);

//查找元素e的直接前驱
bool PriorElem(SqList L, ElemType cur_e, ElemType &pre_e);

//查找元素e直接后驱
bool NextElem(SqList L, ElemType cur_e, ElemType &next_e);

//插入节点
bool ListInsert(SqList &L, int i, ElemType e);

//删除第i个节点
bool ListDelete(SqList &L, int i, ElemType &e);

void ListReverse(SqList &L);

bool IsPalindrome(SqList L);

bool SearchSubList(SqList L, SqList pattern);

void MergeList(SqList &L, SqList pattern);