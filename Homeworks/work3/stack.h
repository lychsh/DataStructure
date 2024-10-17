/*
实现顺序栈的ADT（抽象数据结构）
节点包含数据域(元素为string的数组)，栈元素个数，栈容量，栈顶和栈底

基于实现的顺序栈实现一些基础功能，如中缀表达式转后缀表达式，括号匹配......
*/

#include<iostream>
#include<stdbool.h>

#define STACK_INIT_SIZE 1000  //顺序栈存储空间的初始分配量
#define STACK_INCREMENT  100//固定增长分配量
#define None 0          //初始化空节点的值

typedef long long elemtype;                   //元素类型

typedef struct{
    int stacksize;   //栈的容量
    elemtype *top;   //栈顶指针
    elemtype *base;  //栈底指针
    elemtype minindex;  //最小元素
}stack;


//节点比较函数
int datacmy(elemtype e1, elemtype e2);

//栈的初始化
void init_stack(stack &S);

//判断栈是否为空
bool stack_empty(stack S);

//栈重申请内存,申请成功自动释放原内存空间
elemtype* stack_realloc(elemtype* src, elemtype* end, size_t size);

//压栈
void push(stack &S, elemtype e);

//出栈
void pop(stack &S, elemtype &e);

//取栈顶元素
void get_top(stack S, elemtype &e);

//销毁栈
void destroy_stack(stack &S);

//清空栈
void clear_stack(stack &S);

//栈长
int stack_length(stack S);

//遍历栈进行visit操作
void stack_traverse(stack &S, void (*visit)(elemtype e));

//返回最小值
void get_min(stack S, elemtype &node);