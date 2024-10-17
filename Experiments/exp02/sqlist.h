#include<stdbool.h>
#include<stdlib.h>
#include<memory.h>
#include<assert.h>
#include<string.h>
#include<iostream>


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
#define CHINESE 2
#define ENGLISH 1

/*
这里实现时只使用了二倍扩容,初始容量为4096, 避免重复扩容
*/
#define INIT_SIZE 4096

typedef std::string elemtype;  // 存放单词
typedef std::string::size_type size_type;    //顺序表元素大小的类型

typedef struct{
    elemtype *elem;   //顺序表
    int length;     //表长(现有元素个数)，初始为0
    int listsize;   //存储容量，能容纳的元素个数
}sqlist;

/*
----------------------------------------------基本操作-----------------------------------------------------
*/

//数据比较函数
int datacompare(elemtype e1, elemtype e2);

//链表重申请内存,申请成功自动释放原内存空间
elemtype* list_realloc(elemtype* src, elemtype* end, size_t size);

//表满扩容
bool capacity_expansion(sqlist &L);

//空间利用率过低，缩容
bool capacity_reduce(sqlist &L);

//顺序表初始化
void init_list(sqlist &L);

//指定大小初始化
void init_list(sqlist &L, size_t size);

//打印顺序表
void list_print(sqlist L);

//顺序表销毁
void destroy_list(sqlist &L);

//清空顺序表
void clear_list(sqlist &L);

//判断顺序表是否为空
bool list_empty(sqlist L);

//获取顺序表长度
int list_length(sqlist L);

//获取第i个元素
void get_elem(sqlist L, int i, elemtype &e);

//查找元素e的位置
int locate_elem(sqlist L, elemtype e);

//查找元素e的直接前驱
void prior_elem(sqlist L, elemtype cur_e, elemtype &pre_e);

//查找元素e直接后驱
void next_elem(sqlist L, elemtype cur_e, elemtype &next_e);

//插入节点
void list_insert(sqlist &L, int i, elemtype e);

//删除第i个节点
void list_delete(sqlist &L, int i, elemtype &e);


/*
--------------------------------------------初级功能-----------------------------------------------------
*/

//返回从start开始第一个分隔符的位置,分隔符包含中文和英文
size_type find_first_delimiter(elemtype str, elemtype::size_type size, elemtype::size_type start, int &language);

//返回从start开始第一个匹配子串的位置，找不到返回size,有空实现kmp算法
size_type find_first_substr(std::string str, std::string pattern, size_type start, size_type size);

//返回从start开始第一个分隔符的位置，传入参数delimiters作为分隔符
size_type find_first_delimiter(elemtype str, elemtype::size_type size, elemtype::size_type start, std::string delimiters);

//创造顺序表
void create_list(sqlist &S, std::string str);

//覆盖原字符串
void coverlist(sqlist &L, elemtype str);

//从键盘输入链表内容(尾插)
void create_list_from_input(sqlist &L);

//从文件读入链表内容(尾插)
void create_list_from_file(sqlist &L, std::string path);

